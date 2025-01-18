#include "ps2.h"
#include "../../libc/io.h"
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


struct ps2_state ps2_state;


static struct ps2_status_reg ps2_read_status_reg() {
    uint8_t sr = inb(PS2_STATUS_REG);
    return *((struct ps2_status_reg*)&sr);
}

/// @brief stops code execution until PS2's output-buffer-status (bit0) is set
static void ps2_wait_for_read() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.output_buffer_status == 0);
}

/// @brief stops code execution until PS2's output-buffer-status (bit0) is empty
static void ps2_wait_for_empty_output_buffer() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.output_buffer_status == 1);
}

/// @brief stops code execution until PS2's input-buffer-status (bit1) is clear
static void ps2_wait_for_write() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.input_buffer_status == 1);
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


uint8_t ps2_ram_read_byte_0() {
    return ps2_responsive_send_cmd(0x20);
}
struct ps2_controller_config ps2_read_controller_config() {
    uint8_t cc = ps2_responsive_send_cmd(0x20);
    return *((struct ps2_controller_config*)&cc);
}

uint8_t ps2_ram_read_byte_n(uint8_t n) {
    // there are only addresses 0x00 to 0x1f in PS2's internal RAM
    if (n > 0x1f) return 0x00;

    return ps2_responsive_send_cmd(0x20+n);
}

void ps2_ram_write_byte_0(uint8_t byte) {
    ps2_send_cmd_arg(0x60, byte);
}
void ps2_write_controller_config(struct ps2_controller_config cc) {
    ps2_send_cmd_arg(0x60, *((uint8_t*)&cc));
}

void ps2_ram_write_byte_n(uint8_t n, uint8_t byte) {
    // there are only addresses 0x00 to 0x1f in PS2's internal RAM
    if (n > 0x1f) return;

    ps2_send_cmd_arg(0x20+n, byte);
}

// only if 2 PS2 ports supported
void ps2_disable_second_ps2_port() {
    ps2_send_cmd(0xa7);
}

// only if 2 PS2 ports supported
void ps2_enable_second_ps2_port() {
    ps2_send_cmd(0xa8);
}

/// only if 2 PS2 ports supported
/// @returns 0x00 = test passed ; 0x01 = clock line stuck low ; 0x02 = clock line stuck high ; 0x03 = data line stuck low ; 0x04 = data line stuck high
uint8_t ps2_test_second_ps2_port() {
    return ps2_responsive_send_cmd(0xa9);
}

/// @returns 0x55 = test passed ; 0xfc = test failed
uint8_t ps2_test_ps2_contoller() {
    return ps2_responsive_send_cmd(0xaa);
}

/// @returns 0x00 = test passed ; 0x01 = clock line stuck low ; 0x02 = clock line stuck high ; 0x03 = data line stuck low ; 0x04 = data line stuck high
uint8_t ps2_test_first_ps2_port() {
    return ps2_responsive_send_cmd(0xab);
}

// 0xac - diagnostic dump (read all byte of internal ram)

void ps2_disable_first_ps2_port() {
    ps2_send_cmd(0xad);
}

void ps2_enable_first_ps2_port() {
    ps2_send_cmd(0xae);
}

/// @return unkown (none of the bits have a standard/defined purpose)
uint8_t ps2_read_controller_input_port() {
    return ps2_responsive_send_cmd(0xc0);
}

void ps2_copy_input_port_low_to_status_high() {
    ps2_send_cmd(0xc1);
}

void ps2_copy_input_port_high_to_status_high() {
    ps2_send_cmd(0xc2);
}

struct ps2_controller_output_port ps2_read_controller_output_port() {
    uint8_t cop = ps2_responsive_send_cmd(0xd0);
    return *((struct ps2_controller_output_port*)&cop);
}

void ps2_write_to_controller_output_port(struct ps2_controller_output_port cop) {
    ps2_wait_for_write();
    outb(PS2_CMD_REG, 0xd1);
    ps2_wait_for_write();
    ps2_wait_for_empty_output_buffer();
    outb(PS2_DATA_PORT, *((uint8_t*)&cop));
}

// only if 2 PS2 ports supported
// makes it look like the byte written was received from the first PS/2 port
void ps2_write_to_first_ps2_port_output_buffer(uint8_t byte) {
    ps2_send_cmd_arg(0xd2, byte);
}

// only if 2 PS2 ports supported
// makes it look like the byte written was received from the second PS/2 port
void ps2_write_to_second_ps2_port_output_buffer(uint8_t byte) {
    ps2_send_cmd_arg(0xd3, byte);
}

// only if 2 PS2 ports supported
void ps2_write_to_second_ps2_port_input_buffer(uint8_t byte) {
    ps2_send_cmd_arg(0xd4, byte);
}

/// pulsed lines low for 6ms according to mask.
/// bit 0 corresponds to the "reset" line. the other output lines don't have a standard/defined purpose
/// @param mask a 4 bit mask (0 = pulse line, 1 = don't pulse line). correspond to 4 different output lines
void pulse_output_lines_low(uint8_t mask) {
    ps2_send_cmd(0xf0 | (mask & 0x0f));
}


uint8_t ps2_init() {
    struct ps2_controller_config cc;
    
    // TODO: how tf do i disable/enable usb legacy support???

    // assuming ps2 controller exists (beacuse i cant fucking bother to (TODO: ) write an ACPI driver)

    // disable devices so that they won't send data during initialization
    ps2_disable_second_ps2_port(); // will be ignored if is a signle channel device
    ps2_disable_first_ps2_port();

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
        ps2_state.controller_working = 0;
        return 1;
    }
    ps2_state.controller_working = 1;

    // determine if there are 2 channels
    // enable second port to check controller config
    ps2_enable_second_ps2_port();
    // controllec config bit1 (second_ps2_port_clock) indicates wether or not this is a dual device
    cc = ps2_read_controller_config();
    ps2_state.is_dual = (cc.second_ps2_port_clock == 0);
    if (ps2_state.is_dual == 1) {
        // redisable second device
        ps2_disable_second_ps2_port();
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
    if (ps2_test_first_ps2_port() == 0x00) {
        ps2_state.first_port_working = 1;
    }else {
        ps2_state.first_port_working = 0;
    }
    // test second port (if dual)
    if (ps2_state.is_dual == 1) {
        if (ps2_test_second_ps2_port() == 0x00) {
            ps2_state.second_port_working = 1;
        }else {
            ps2_state.second_port_working = 0;
        }
    }else {
        ps2_state.second_port_working = 0;
    }
    // if both ports dont work, give up
    if (ps2_state.first_port_working == 0 && ps2_state.second_port_working == 0) {
        // error
        return 2;
    }
    
    // reenable devices
    if (ps2_state.first_port_working == 1){
        ps2_enable_first_ps2_port();
    }
    if (ps2_state.second_port_working == 1){
        ps2_enable_second_ps2_port();
    }
    
    // reenable IRQs
    cc = ps2_read_controller_config();
    if (ps2_state.first_port_working == 1) {
        cc.first_ps2_port_interrupt = 1;
    }
    if (ps2_state.second_port_working == 1) {
        cc.second_ps2_port_interrupt = 1;
    }
    ps2_write_controller_config(cc);

    // reset devices
    /* TODO:
        Once controller initialisation is done, all PS/2 devices (if any) should be reset by the driver.
        This can be done by sending byte 0xFF (reset command, supported by all PS/2 devices) to port 1 (and
        port 2 for dual channel controllers, see "Sending Bytes to Device/s") and waiting for a response.
        If the response is 0xFA, 0xAA (Note: the order in which devices send these two seems to be
        ambiguous) followed by the device PS/2 ID (see "Detecting PS/2 Device Types" for a list of IDs),
        then the device reset was successful. If the response is 0xFC then the self test failed and the
        device should be ignored. If theres no response at all then the port is not populated.
    */

    // init success
    return 0;
}
