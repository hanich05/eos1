#include "ps2.h"
#include "../../libc/io.h"
#include "../../kernel/interrupts/irqs/timer/timer.h"
#include "../../kernel/text_mode.h"
#include <stdint.h>


// read/write
// used for reading data that was received from a PS2 device or from the PS2 controller itself
// and writing data to PS2 device or the PS2 controller itself
#define PS2_DATA_PORT   0x60
// read
// contains a struct ps2_status_reg
#define PS2_STATUS_REG  0x64
// write
// used for sending commands to the PS2 controller (not to PS2 devices)
#define PS2_CMD_REG     0x64


__attribute__((packed))
struct ps2_status_reg {
    // 0 = empty, 1 = full
    // must be set before attempting to read data from PS2_STATUS_REG
    uint8_t output_buffer_status    : 1;
    // 0 = empty, 1 = full
    // must be clear before attempting to write data to PS2_DATA_PORT or PS2_CMD_REG
    uint8_t input_buffer_status     : 1;
    // meant to be cleared on reset and set by firmware
    uint8_t system_flag             : 1;
    uint8_t bit4_chipset_specific   : 1;
    uint8_t bit5_chipset_specific   : 1;
    // 0 = no error, 1 = time-out error
    uint8_t time_out_error          : 1;
    // 0 = no error, 1 = parity error
    uint8_t parity_error            : 1;
};

__attribute__((packed))
struct ps2_controller_config {
    // 1 = enabled, 0 = disabled
    uint8_t first_ps2_port_interrupt    : 1;
    // 1 = enabled, 0 = disabled
    // only if 2 PS2 ports supported
    uint8_t second_ps2_port_interrupt   : 1;
    // 1 = system passed POST, 0 = your OS shouldn't be running
    uint8_t system_flag                 : 1;
    uint8_t should_be_zero              : 1;
    // 1 = disabled, 0 = enabled
    uint8_t first_ps2_port_clock        : 1;
    // 1 = disabled, 0 = enabled
    // only if 2 PS2 ports supported
    uint8_t second_ps2_port_clock       : 1;
    // 1 = enabled, 0 = disabled
    uint8_t first_ps2_port_translation  : 1;
    uint8_t must_be_zero                : 1;
};

__attribute__((packed))
struct ps2_controller_output_port {    
    // always set to 1
    // to reset system, pulse the reset line using pulse_output_lines_low (command 0xfe).
    // setting this bit to 0 can lock the computer up ("reset forever")
    uint8_t system_reset                                        : 1;
    uint8_t a20_gate                                            : 1;
    // only if 2 PS2 ports supported
    uint8_t second_ps2_port_clock                               : 1;
    // only if 2 PS2 ports supported
    uint8_t second_ps2_port_data                                : 1;
    // connected to IRQ1
    uint8_t output_buffer_full_with_byte_from_first_ps2_port    : 1;
    // connected to IRQ12
    // only if 2 PS2 ports supported
    uint8_t output_buffer_full_with_byte_from_second_ps2_port   : 1;
    uint8_t first_ps2_port_clock                                : 1;
    uint8_t first_ps2_port_data                                 : 1;
};

struct ps2_device {
    uint8_t port_working                    : 1;
    uint8_t device_working                  : 1;
    uint8_t timed_out                       : 1;
    uint8_t long_device_id                  : 1;
    uint8_t device_driver_not_found         : 1;
    uint8_t input_arrived_since_last_echo   : 1;
    uint8_t echo_succeeded                  : 1;
    uint8_t device_id[2];
    uint8_t reset_response[2];
    uint16_t device_driver; // ps2_device_drivers_amount if not set yet
};


static struct ps2_status_reg ps2_read_status_reg();
static struct ps2_controller_config ps2_read_controller_config();
static struct ps2_controller_output_port ps2_read_controller_output_port();
static void ps2_write_controller_config(struct ps2_controller_config cc);
static void ps2_write_to_controller_output_port(struct ps2_controller_output_port cop);

/// @brief stops code execution until PS2's output-buffer-status (bit0) is set
static void ps2_wait_for_read();
/// @brief stops code execution until PS2's output-buffer-status (bit0) is set.
/// Times out after ps2_read_timeout ticks
/// @returns 1 if timed-out, otherwise 0
static uint8_t ps2_wait_for_read_with_timeout();
/// @brief stops code execution until PS2's output-buffer-status (bit0) is empty
static void ps2_wait_for_empty_output_buffer();
/// @brief stops code execution until PS2's input-buffer-status (bit1) is clear
static void ps2_wait_for_write();
/// @brief stops code execution until PS2's input-buffer-status (bit1) is clear.
/// Times out after ps2_write_timeout ticks
/// @returns 1 if timed-out, otherwise 0
static uint8_t ps2_wait_for_write_with_timeout();

static void ps2_send_cmd(uint8_t cmd);
static void ps2_send_cmd_arg(uint8_t cmd, uint8_t arg);
static uint8_t ps2_responsive_send_cmd(uint8_t cmd);

static uint8_t ps2_ram_read_byte_0();
static uint8_t ps2_ram_read_byte_n(uint8_t n);
static void ps2_ram_write_byte_0(uint8_t byte);
static void ps2_ram_write_byte_n(uint8_t n, uint8_t byte);

/// @returns 0x55 = test passed ; 0xfc = test failed
static uint8_t ps2_test_ps2_contoller();

/// @return unkown (none of the bits have a standard/defined purpose)
static uint8_t ps2_read_controller_input_port();
static void ps2_copy_input_port_low_to_status_high();
static void ps2_copy_input_port_high_to_status_high();

/// pulsed lines low for 6ms according to mask.
/// bit 0 corresponds to the "reset" line. the other output lines don't have a standard/defined purpose
/// @param mask a 4 bit mask (0 = pulse line, 1 = don't pulse line). correspond to 4 different output lines
static void pulse_output_lines_low(uint8_t mask);

/// waits for 0xfa to arrive in PS2_DATA_PORT
static void ps2_wait_for_ack();

// assumes both devices' interrupts are disabled
// saves response in ps2_devices[device].reset_response
static void ps2_reset_device(uint8_t device);
// assumes both ports' interrupts are disabled
// saves response in ps2_devices[device].device_id
static void ps2_get_device_id(uint8_t device);

uint8_t ps2_init();

static void ps2_send_echo_intervaled_callback();
static void ps2_check_echo_intervaled_callback();


uint8_t ps2_controller_working;
uint8_t ps2_controller_is_dual;
struct ps2_device ps2_devices[2];

static uint32_t ps2_read_timeout;
static uint32_t ps2_write_timeout;
static uint32_t ps2_send_echo_intervaled_callback_uid;
static uint32_t ps2_check_echo_intervaled_callback_uid;

static struct ps2_device_driver* ps2_device_drivers[PS2_MAX_DEVICE_DRIVERS];
static uint16_t ps2_device_drivers_amount = 0;


static struct ps2_status_reg ps2_read_status_reg() {
    uint8_t sr = inb(PS2_STATUS_REG);
    return *((struct ps2_status_reg*)&sr);
}
static struct ps2_controller_config ps2_read_controller_config() {
    uint8_t cc = ps2_responsive_send_cmd(0x20);
    return *((struct ps2_controller_config*)&cc);
}
static struct ps2_controller_output_port ps2_read_controller_output_port() {
    uint8_t cop = ps2_responsive_send_cmd(0xd0);
    return *((struct ps2_controller_output_port*)&cop);
}
static void ps2_write_controller_config(struct ps2_controller_config cc) {
    ps2_send_cmd_arg(0x60, *((uint8_t*)&cc));
}
static void ps2_write_to_controller_output_port(struct ps2_controller_output_port cop) {
    ps2_wait_for_write();
    outb(PS2_CMD_REG, 0xd1);
    ps2_wait_for_write();
    ps2_wait_for_empty_output_buffer();
    outb(PS2_DATA_PORT, *((uint8_t*)&cop));
}

static void ps2_wait_for_read() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.output_buffer_status == 0);
}
static uint8_t ps2_wait_for_read_with_timeout() {
    struct ps2_status_reg sr;
    uint32_t start_tsb = ticks_since_boot;

    // wait until output-buffer-status is full with timeout of ps2_read_timeout
    while ((ticks_since_boot - start_tsb) < ps2_read_timeout) {
        // get status reg
        sr = ps2_read_status_reg();
        
        // continue loop if output-buffer-status is empty
        if (sr.input_buffer_status == 0) continue;
        // output-buffer-status is full

        // success
        return 0;
    }
    // timed-out
    return 1;
}
static void ps2_wait_for_empty_output_buffer() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.output_buffer_status == 1);
}
static void ps2_wait_for_write() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.input_buffer_status == 1);
}
static uint8_t ps2_wait_for_write_with_timeout() {
    struct ps2_status_reg sr;
    uint32_t start_tsb = ticks_since_boot;

    // wait until input-buffer-status is empty with timeout of ps2_write_timeout
    while ((ticks_since_boot - start_tsb) < ps2_write_timeout) {
        // get status reg
        sr = ps2_read_status_reg();
        
        // continue loop if input-buffer-status is full
        if (sr.input_buffer_status == 1) continue;
        // input-buffer-status is empty

        // success
        return 0;
    }
    // timed-out
    return 1;
}

static void ps2_send_cmd(uint8_t cmd) {
    ps2_wait_for_write();
    outb(PS2_CMD_REG, cmd);
}
static void ps2_send_cmd_arg(uint8_t cmd, uint8_t arg) {
    ps2_wait_for_write();
    outb(PS2_CMD_REG, cmd);
    ps2_wait_for_write();
    outb(PS2_DATA_PORT, arg);
}
static uint8_t ps2_responsive_send_cmd(uint8_t cmd) {
    ps2_wait_for_write();
    outb(PS2_CMD_REG, cmd);
    ps2_wait_for_read();
    return inb(PS2_DATA_PORT);
}

static uint8_t ps2_ram_read_byte_0() {
    return ps2_responsive_send_cmd(0x20);
}
static uint8_t ps2_ram_read_byte_n(uint8_t n) {
    // there are only addresses 0x00 to 0x1f in PS2's internal RAM
    if (n > 0x1f) return 0x00;

    return ps2_responsive_send_cmd(0x20+n);
}
static void ps2_ram_write_byte_0(uint8_t byte) {
    ps2_send_cmd_arg(0x60, byte);
}
static void ps2_ram_write_byte_n(uint8_t n, uint8_t byte) {
    // there are only addresses 0x00 to 0x1f in PS2's internal RAM
    if (n > 0x1f) return;

    ps2_send_cmd_arg(0x20+n, byte);
}

static uint8_t ps2_test_ps2_contoller() {
    return ps2_responsive_send_cmd(0xaa);
}

// 0xac - diagnostic dump (read all bytes of internal ram)

static uint8_t ps2_read_controller_input_port() {
    return ps2_responsive_send_cmd(0xc0);
}
static void ps2_copy_input_port_low_to_status_high() {
    ps2_send_cmd(0xc1);
}
static void ps2_copy_input_port_high_to_status_high() {
    ps2_send_cmd(0xc2);
}

/*
// only if 2 PS2 ports supported
// makes it look like the byte written was received from the first PS/2 port
uint8_t ps2_write_to_first_ps2_port_output_buffer(uint8_t byte) {
    ps2_send_cmd_arg(0xd2, byte);
}

// only if 2 PS2 ports supported
// makes it look like the byte written was received from the second PS/2 port
uint8_t ps2_write_to_second_ps2_port_output_buffer(uint8_t byte) {
    ps2_send_cmd_arg(0xd3, byte);
}

// only if 2 PS2 ports supported
uint8_t ps2_write_to_second_ps2_port_input_buffer(uint8_t byte) {
    ps2_send_cmd_arg(0xd4, byte);
}
*/

static void pulse_output_lines_low(uint8_t mask) {
    ps2_send_cmd(0xf0 | (mask & 0x0f));
}

static void ps2_wait_for_ack() {
    uint8_t r;
    do {
        ps2_wait_for_read();
        r = inb(PS2_DATA_PORT);
    }while (r != 0xfa);
}

static void ps2_reset_device(uint8_t device) {
    ps2_devices[device].reset_response[0] = 0x00;
    ps2_devices[device].reset_response[1] = 0x00;

    // send reset command
    if (ps2_send_byte(device, 0xff) != 0) {
        // timed out
        ps2_devices[device].device_working = 0;
        ps2_devices[device].timed_out = 1;
        return;
    }

    // read response (should be 0xfa then 0xaa or 0xaa then 0xfa if valid)

    ps2_wait_for_read();
    ps2_devices[device].reset_response[0] = inb(PS2_DATA_PORT);
    if (ps2_devices[device].reset_response[0] == 0xfc) {
        // self test failed
        ps2_devices[device].device_working = 0;
        return;
    }
    if (ps2_devices[device].reset_response[0] != 0xfa && ps2_devices[device].reset_response[0] != 0xaa) {
        // invalid response
        ps2_devices[device].device_working = 0;
        return;
    }
    
    ps2_wait_for_read();
    ps2_devices[device].reset_response[1] = inb(PS2_DATA_PORT);
    if (
        (ps2_devices[device].reset_response[1] != 0xfa && ps2_devices[device].reset_response[1] != 0xaa) ||
        ps2_devices[device].reset_response[0] == ps2_devices[device].reset_response[1]) {
        // invalid response
        ps2_devices[device].device_working = 0;
        return;
    }
    
    // flush device-id input that may or may not arrive
    while (ps2_wait_for_read_with_timeout() == 0) {
        inb(PS2_DATA_PORT);
    }
}
static void ps2_get_device_id(uint8_t device) {
    ps2_devices[device].long_device_id = 0;
    ps2_devices[device].device_id[0] = 0x00;
    ps2_devices[device].device_id[1] = 0x00;

    // send Disable-Scanning command
    if (ps2_disable_scanning(device) != 0) {
        // timed out
        ps2_devices[0].device_working = 0;
        ps2_devices[0].timed_out = 1;
        return;
    }
    // wait for ACK response
    ps2_wait_for_ack();
    
    // send Identity command
    if (ps2_send_byte(device, 0xf2) != 0) {
        // timed out
        ps2_devices[0].device_working = 0;
        ps2_devices[0].timed_out = 1;
        return;
    }
    // wait for ACK response
    ps2_wait_for_ack();

    // get first device-id byte
    ps2_wait_for_read();
    ps2_devices[device].device_id[0] = inb(PS2_DATA_PORT);

    // get device-id byte (with time out beacuse it may not arrive)
    if (ps2_wait_for_read_with_timeout() == 0) {
        ps2_devices[device].long_device_id = 1;
        ps2_devices[device].device_id[1] = inb(PS2_DATA_PORT);
    }

    // send Enable-Scanning command
    if (ps2_enable_scanning(device) != 0) {
        // timed out
        ps2_devices[0].device_working = 0;
        ps2_devices[0].timed_out = 1;
        return;
    }
    ps2_wait_for_ack();
}
uint8_t ps2_init() {
    struct ps2_controller_config cc;

    ps2_read_timeout  = PS2_DEFAULT_READ_TIMEOUT;
    ps2_write_timeout = PS2_DEFAULT_WRITE_TIMEOUT;
    ps2_send_echo_intervaled_callback_uid = 0; // doesn't exist yet
    ps2_check_echo_intervaled_callback_uid = 0; // doesn't exist yet
    
    // TODO: how tf do i disable/enable usb legacy support???

    // assuming ps2 controller exists (beacuse i cant fucking bother to (TODO: ) write an ACPI driver)

    // disable devices' ports so that they won't send data during initialization
    ps2_disable_port(1); // will be ignored if is a signle channel device
    ps2_disable_port(0);

    // flush the output buffer
    inb(PS2_DATA_PORT);

    // set the controller config
    cc = ps2_read_controller_config();
    // disable first port's IRQs
    cc.first_ps2_port_interrupt = 0;
    // disable first port's translation
    cc.first_ps2_port_translation = 0;
    // enable first port's clock signal
    cc.first_ps2_port_clock = 0;
    // update controller config
    ps2_write_controller_config(cc);

    // preform controller self test
    if (ps2_test_ps2_contoller() != 0x55) {
        // error
        ps2_controller_working = 0;
        return 1;
    }
    ps2_controller_working = 1;

    // determine if there are 2 channels
    // enable second port to check controller config
    ps2_enable_port(1);
    // controllec config bit1 (second_ps2_port_clock) indicates wether or not this is a dual device
    cc = ps2_read_controller_config();
    ps2_controller_is_dual = (cc.second_ps2_port_clock == 0);
    if (ps2_controller_is_dual == 1) {
        // redisable second device's port
        ps2_disable_port(1);
        // disable second port's IRQs
        cc.second_ps2_port_interrupt = 0;
        // translations are never supported by the seccond port
        // enable second port's clock
        cc.second_ps2_port_clock = 0;
        // update controller config
        ps2_write_controller_config(cc);
    }

    // preform interface tests
    // test first port
    if (ps2_test_port(0) == 0x00) {
        ps2_devices[0].port_working = 1;
        ps2_devices[0].device_working = 1;
    }else {
        ps2_devices[0].port_working = 0;
        ps2_devices[0].device_working = 0;
    }
    // test second port (if dual)
    if (ps2_controller_is_dual == 1) {
        if (ps2_test_port(1) == 0x00) {
            ps2_devices[1].port_working = 1;
            ps2_devices[1].device_working = 1;
        }else {
            ps2_devices[1].port_working = 0;
            ps2_devices[1].device_working = 0;
        }
    }else {
        ps2_devices[1].port_working = 0;
        ps2_devices[1].device_working = 0;
    }
    // if both ports dont work, give up
    if (ps2_devices[0].port_working == 0 && ps2_devices[1].port_working == 0) {
        // error
        return 2;
    }
    
    // reenable ports
    if (ps2_devices[0].port_working == 1){
        ps2_enable_port(0);
    }
    if (ps2_devices[1].port_working == 1){
        ps2_enable_port(1);
    }

    // reset devices (updates ps2_devices[x].device_working)
    if (ps2_devices[0].port_working == 1) {
        ps2_reset_device(0);
    }
    if (ps2_devices[1].port_working == 1) {
        ps2_reset_device(1);
    }

    // get device ids (updates ps2_devices[x].device_working)
    if (ps2_devices[0].device_working == 1) {
        ps2_get_device_id(0);
    }
    if (ps2_devices[1].device_working == 1) {
        ps2_get_device_id(1);
    }

    // reenable IRQs
    cc = ps2_read_controller_config();
    if (ps2_devices[0].port_working == 1) {
        cc.first_ps2_port_interrupt = 1;
    }
    if (ps2_devices[1].port_working == 1) {
        cc.second_ps2_port_interrupt = 1;
    }
    ps2_write_controller_config(cc);

    // set intervaled callback
    ps2_send_echo_intervaled_callback_uid = install_intervaled_callback(&ps2_send_echo_intervaled_callback, PS2_DEFAULT_ECHO_INTERVAL, 0);
    ps2_check_echo_intervaled_callback_uid = install_intervaled_callback(&ps2_check_echo_intervaled_callback, PS2_DEFAULT_ECHO_INTERVAL, PS2_DEFAULT_ECHO_TIMEOUT);

    print_string("\nps2 initialized:", TM_FORE_COL_GRAY);
    for (uint8_t i = 0; i < 2; i++) {
        print_string("\n- port ", TM_FORE_COL_GRAY);
        print_hex8(i, TM_FORE_COL_MAGENTA);
        if (ps2_devices[i].port_working == 0) {
            print_string(" doesn't work", TM_FORE_COL_RED);
        }else {
            print_string(" works", TM_FORE_COL_GRAY);
        }
        print_string("\n- device ", TM_FORE_COL_GRAY);
        print_hex8(i, TM_FORE_COL_MAGENTA);
        if (ps2_devices[i].device_working == 0) {
            print_string(" doesn't work", TM_FORE_COL_RED);
        }else {
            print_string(" works with device-id ", TM_FORE_COL_GRAY);
            print_hex8(ps2_devices[i].device_id[0], TM_FORE_COL_CYAN);
            if (ps2_devices[i].long_device_id == 1) {
                print_char(' ', 0);
                print_hex8(ps2_devices[i].device_id[1], TM_FORE_COL_CYAN);
            }
        }
    }

    // init success
    return 0;
}

void ps2_install_device_driver(struct ps2_device_driver* driver) {
    if (ps2_device_drivers_amount >= PS2_MAX_DEVICE_DRIVERS) return;
    
    ps2_device_drivers[ps2_device_drivers_amount] = driver;

    if (ps2_devices[0].device_driver == ps2_device_drivers_amount) ps2_devices[0].device_driver++;
    if (ps2_devices[1].device_driver == ps2_device_drivers_amount) ps2_devices[1].device_driver++;
    
    ps2_device_drivers_amount++;

    // tell devices that dont have a device driver to look for a device driver again
    ps2_devices[0].device_driver_not_found = 0;
    ps2_devices[1].device_driver_not_found = 0;
}
static uint16_t ps2_device_id_find_driver(uint8_t long_device_id, uint8_t* device_id) {
    // check for long absolute match
    if (long_device_id == 1) {
        for (uint16_t i = 0; i < ps2_device_drivers_amount; i++) {
            if (
                ps2_device_drivers[i]->long_device_id == 1          &&
                ps2_device_drivers[i]->device_id[0] == device_id[0] &&
                ps2_device_drivers[i]->device_id[1] == device_id[1]
            ) {
                return i;
            }
        }
    }
    // check for long partial match and short absolute match
    for (uint16_t i = 0; i < ps2_device_drivers_amount; i++) {
        if (ps2_device_drivers[i]->device_id[0] == device_id[0]) {
            return i;
        }
    }
    return ps2_device_drivers_amount;
}
static void ps2_device_find_driver(uint8_t device) {
    // already searched
    if (ps2_devices[device].device_driver_not_found == 1) return;
    // already found
    if (ps2_devices[device].device_driver != ps2_device_drivers_amount) return;

    // find
    ps2_devices[device].device_driver = ps2_device_id_find_driver(ps2_devices[device].long_device_id, ps2_devices[device].device_id);
    
    // did not find
    if (ps2_devices[device].device_driver == ps2_device_drivers_amount) {
        ps2_devices[device].device_driver_not_found = 1;
        return;
    }

    // found - init device driver
    uint8_t driver_on_connect_response = ps2_device_drivers[ps2_devices[device].device_driver]->on_connect(device);
    // check if device is working
    if (driver_on_connect_response != 0) {
        print_string("\ndevice driver ", TM_FORE_COL_RED);
        print_hex8(ps2_devices[device].device_driver, TM_FORE_COL_MAGENTA);
        print_string(" encountered an error when connecting to device ", TM_FORE_COL_RED);
        print_hex8(device, TM_FORE_COL_MAGENTA);
        ps2_devices[device].device_working = 0;
    }
}

void ps2_set_read_timeout(uint32_t to) {
    ps2_read_timeout = to;
}
void ps2_set_write_timeout(uint32_t to) {
    ps2_write_timeout = to;
}
uint32_t ps2_get_read_timeout() {
    return ps2_read_timeout;
}
uint32_t ps2_get_write_timeout() {
    return ps2_write_timeout;
}

uint8_t ps2_send_byte(uint8_t device, uint8_t byte) {
    if (device >= 2) return 2;

    if (device == 1) {
        // write next byte to second PS2 port's input buffer
        ps2_send_cmd(0xd4);
    }

    // wait for write - return 1 if timed out
    if (ps2_wait_for_write_with_timeout() == 1) return 1;

    // write byte
    outb(PS2_DATA_PORT, byte);

    // success
    return 0;
}
uint8_t ps2_receive_byte(uint8_t* byte) {
    if (ps2_wait_for_read_with_timeout() == 1) {
        // timed out
        return 1;
    }
    *byte = inb(PS2_DATA_PORT);
    return 0;
}

void ps2_enable_port(uint8_t port) {
    if (port == 0) {
        ps2_send_cmd(0xae);
    }
    if (port == 1) {
        ps2_send_cmd(0xa8);
    }
}
void ps2_disable_port(uint8_t port) {
    if (port == 0) {
        ps2_send_cmd(0xad);
    }
    if (port == 1) {
        ps2_send_cmd(0xa7);
    }
}
uint8_t ps2_test_port(uint8_t port) {
    if (port == 0) {
        // 0x00 = test passed ; 0x01 = clock line stuck low ; 0x02 = clock line stuck high ; 0x03 = data line stuck low ; 0x04 = data line stuck high
        return (ps2_responsive_send_cmd(0xab) != 0);
    }
    if (port == 1) {
        // 0x00 = test passed ; 0x01 = clock line stuck low ; 0x02 = clock line stuck high ; 0x03 = data line stuck low ; 0x04 = data line stuck high
        return (ps2_responsive_send_cmd(0xa9) != 0);
    }
    // port does not exist
    return 2;
}

uint8_t ps2_disable_scanning(uint8_t device) {
    return ps2_send_byte(device, 0xf5);
}
uint8_t ps2_enable_scanning(uint8_t device) {
    return ps2_send_byte(device, 0xf4);
}

void ps2_declare_echo_success(uint8_t device) {
    ps2_devices[device].echo_succeeded = 1;
}
static void ps2_send_echo_intervaled_callback() {
    if (ps2_devices[0].device_working == 1) {
        ps2_device_find_driver(0);
        if (ps2_devices[0].device_driver_not_found == 0) {
            if (ps2_devices[0].input_arrived_since_last_echo == 0) {
                // echo needed
                ps2_device_drivers[ps2_devices[0].device_driver]->send_echo(0);
                ps2_devices[0].echo_succeeded = 0;
            }else {
                // device has communicated in the last (ps2_echo_interval-ps2_echo_timeout) ticks. no need for echo
                ps2_devices[0].echo_succeeded = 1;
            }
        }
    }

    if (ps2_devices[1].device_working == 1) {
        ps2_device_find_driver(1);
        if (ps2_devices[1].device_driver_not_found == 0) {
            if (ps2_devices[1].input_arrived_since_last_echo == 0) {
                // echo needed
                ps2_device_drivers[ps2_devices[1].device_driver]->send_echo(1);
                ps2_devices[1].echo_succeeded = 0;
            }else {
                // device has communicated in the last (ps2_echo_interval-ps2_echo_timeout) ticks. no need for echo
                ps2_devices[1].echo_succeeded = 1;
            }
        }
    }
}
static void ps2_check_echo_intervaled_callback() {
    if (ps2_devices[0].echo_succeeded == 0 && ps2_devices[0].device_driver_not_found == 0) {
        print_string("\ndevice 0 did not echo back", TM_FORE_COL_RED);
    }
    if (ps2_devices[1].echo_succeeded == 0 && ps2_devices[1].device_driver_not_found == 0) {
        print_string("\ndevice 1 did not echo back", TM_FORE_COL_RED);
    }
    ps2_devices[0].input_arrived_since_last_echo = 0;
    ps2_devices[1].input_arrived_since_last_echo = 0;
}

static void ps2_handle_device_irq(uint8_t device) {
    uint8_t data = inb(PS2_DATA_PORT);

    if (ps2_devices[device].device_working == 0) {
        print_string("\nreceived ", TM_FORE_COL_RED);
        print_hex8(data, TM_FORE_COL_BROWN);
        print_string(" from non working ps2 device ", TM_FORE_COL_RED);
        print_hex8(device, TM_FORE_COL_MAGENTA);
        return;
    }

    ps2_devices[device].input_arrived_since_last_echo = 1;

    ps2_device_find_driver(device);
    if (ps2_devices[device].device_driver_not_found == 1) {
        print_string("\nreceived ", TM_FORE_COL_GRAY);
        print_hex8(data, TM_FORE_COL_BROWN);
        print_string(" from ps2 device ", TM_FORE_COL_GRAY);
        print_hex8(device, TM_FORE_COL_MAGENTA);
        print_string(" (no device driver yet)", TM_FORE_COL_GRAY);
        return;
    }
    uint8_t irq_handler_response = ps2_device_drivers[ps2_devices[device].device_driver]->irq_handler(device, data);
    // check if device is working
    if (irq_handler_response != 0) {
        print_string("\ndevice driver ", TM_FORE_COL_RED);
        print_hex8(ps2_devices[device].device_driver, TM_FORE_COL_MAGENTA);
        print_string(" encountered an error when handling an irq from device ", TM_FORE_COL_RED);
        print_hex8(device, TM_FORE_COL_MAGENTA);
        ps2_devices[device].device_working = 0;
    }
}
void irq1_handler() {
    ps2_handle_device_irq(0);
}
void irq12_handler() {
    ps2_handle_device_irq(1);
}