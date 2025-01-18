#include "ps2.h"
#include "../../libc/io.h"
#include "../../kernel/interrupts/irqs/timer/timer.h"
#include "../../kernel/text_mode.h"
#include <stdint.h>


struct ps2_state ps2_state;


struct ps2_status_reg ps2_read_status_reg() {
    uint8_t sr = inb(PS2_STATUS_REG);
    return *((struct ps2_status_reg*)&sr);
}


void ps2_wait_for_read() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.output_buffer_status == 0);
}

uint8_t ps2_wait_for_read_with_timeout() {
    struct ps2_status_reg sr;
    uint32_t start_tsb = ticks_since_boot;

    // wait until output-buffer-status is full with timeout of PS2_READ_TIMEOUT
    while ((ticks_since_boot - start_tsb) < PS2_READ_TIMEOUT) {
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

void ps2_wait_for_empty_output_buffer() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.output_buffer_status == 1);
}

void ps2_wait_for_write() {
    struct ps2_status_reg sr;
    do {
        sr = ps2_read_status_reg();
    }while (sr.input_buffer_status == 1);
}

uint8_t ps2_wait_for_write_with_timeout() {
    struct ps2_status_reg sr;
    uint32_t start_tsb = ticks_since_boot;

    // wait until input-buffer-status is empty with timeout of PS2_WRITE_TIMEOUT
    while ((ticks_since_boot - start_tsb) < PS2_WRITE_TIMEOUT) {
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


void ps2_send_cmd(uint8_t cmd) {
    ps2_wait_for_write();
    outb(PS2_CMD_REG, cmd);
}

void ps2_send_cmd_arg(uint8_t cmd, uint8_t arg) {
    ps2_wait_for_write();
    outb(PS2_CMD_REG, cmd);
    ps2_wait_for_write();
    outb(PS2_DATA_PORT, arg);
}

uint8_t ps2_responsive_send_cmd(uint8_t cmd) {
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

void ps2_disable_second_ps2_port() {
    ps2_send_cmd(0xa7);
}

void ps2_enable_second_ps2_port() {
    ps2_send_cmd(0xa8);
}

uint8_t ps2_test_second_ps2_port() {
    return ps2_responsive_send_cmd(0xa9);
}

uint8_t ps2_test_ps2_contoller() {
    return ps2_responsive_send_cmd(0xaa);
}

uint8_t ps2_test_first_ps2_port() {
    return ps2_responsive_send_cmd(0xab);
}

// 0xac - diagnostic dump (read all bytes of internal ram)

void ps2_disable_first_ps2_port() {
    ps2_send_cmd(0xad);
}

void ps2_enable_first_ps2_port() {
    ps2_send_cmd(0xae);
}

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

void pulse_output_lines_low(uint8_t mask) {
    ps2_send_cmd(0xf0 | (mask & 0x0f));
}


uint8_t ps2_send_to_first_port(uint8_t byte) {
    // wait for write - return 1 if timed out
    if (ps2_wait_for_write_with_timeout() == 1) return 1;

    // write byte
    outb(PS2_DATA_PORT, byte);
    
    // success
    return 0;
}

uint8_t ps2_send_to_second_port(uint8_t byte) {
    // Write next byte to second PS/2 port input buffer
    ps2_send_cmd(0xd4);
    
    // wait for write - return 1 if timed out
    if (ps2_wait_for_write_with_timeout() == 1) return 1;
    
    // write byte
    outb(PS2_DATA_PORT, byte);
    
    // success
    return 0;
}


void ps2_wait_for_ack() {
    uint8_t r;
    do {
        ps2_wait_for_read();
        r = inb(PS2_DATA_PORT);
    }while (r != 0xfa);
}


void ps2_reset_first_port() {
    ps2_state.first_port_reset_response[0] = 0x00;
    ps2_state.first_port_reset_response[1] = 0x00;

    // send reset command
    if (ps2_send_to_first_port(0xff) != 0) {
        // timed out
        ps2_state.first_port_working = 0;
        ps2_state.first_port_timed_out = 1;
        return;
    }

    // read response (should be 0xfa then 0xaa or 0xaa then 0xfa if valid)

    ps2_wait_for_read();
    ps2_state.first_port_reset_response[0] = inb(PS2_DATA_PORT);
    if (ps2_state.first_port_reset_response[0] == 0xfc) {
        // self test failed
        ps2_state.first_port_working = 0;
        return;
    }
    if (ps2_state.first_port_reset_response[0] != 0xfa && ps2_state.first_port_reset_response[0] != 0xaa) {
        // invalid response
        ps2_state.first_port_working = 0;
        return;
    }
    
    ps2_wait_for_read();
    ps2_state.first_port_reset_response[1] = inb(PS2_DATA_PORT);
    if (
        (ps2_state.first_port_reset_response[1] != 0xfa && ps2_state.first_port_reset_response[1] != 0xaa) ||
        ps2_state.first_port_reset_response[0] == ps2_state.first_port_reset_response[1]) {
        // invalid response
        ps2_state.first_port_working = 0;
        return;
    }
    
    // flush device-id input that may or may not arrive
    while (ps2_wait_for_read_with_timeout() == 0) {
        inb(PS2_DATA_PORT);
    }
}

void ps2_reset_second_port() {
    ps2_state.second_port_reset_response[0] = 0x00;
    ps2_state.second_port_reset_response[1] = 0x00;

    // send reset command
    if (ps2_send_to_second_port(0xff) != 0) {
        // timed out
        ps2_state.second_port_working = 0;
        ps2_state.second_port_timed_out = 1;
        return;
    }

    // read response (should be 0xfa then 0xaa or 0xaa then 0xfa if valid)

    ps2_wait_for_read();
    ps2_state.second_port_reset_response[0] = inb(PS2_DATA_PORT);
    if (ps2_state.second_port_reset_response[0] == 0xfc) {
        // self test failed
        ps2_state.second_port_working = 0;
        return;
    }
    if (ps2_state.second_port_reset_response[0] != 0xfa && ps2_state.second_port_reset_response[0] != 0xaa) {
        // invalid response
        ps2_state.second_port_working = 0;
        return;
    }

    ps2_wait_for_read();
    ps2_state.second_port_reset_response[1] = inb(PS2_DATA_PORT);
    if (
        (ps2_state.second_port_reset_response[1] != 0xfa && ps2_state.second_port_reset_response[1] != 0xaa) ||
        ps2_state.second_port_reset_response[0] == ps2_state.second_port_reset_response[1]) {
        // invalid response
        ps2_state.second_port_working = 0;
        return;
    }
    
    // flush device-id input that may or may not arrive
    while (ps2_wait_for_read_with_timeout() == 0) {
        inb(PS2_DATA_PORT);
    }
}

void ps2_get_first_port_device_id() {
    uint8_t response;

    ps2_state.first_port_device_id[0] = 0x00;
    ps2_state.first_port_device_id[1] = 0x00;

    // send Disable-Scanning command
    if (ps2_send_to_first_port(0xf5) != 0) {
        // timed out
        ps2_state.first_port_working = 0;
        ps2_state.first_port_timed_out = 1;
        return;
    }
    // wait for ACK response
    ps2_wait_for_ack();
    
    // send Identity command
    if (ps2_send_to_first_port(0xf2) != 0) {
        // timed out
        ps2_state.first_port_working = 0;
        ps2_state.first_port_timed_out = 1;
        return;
    }
    // wait for ACK response
    ps2_wait_for_ack();

    // get first device-id byte
    ps2_wait_for_read();
    ps2_state.first_port_device_id[0] = inb(PS2_DATA_PORT);

    // get second device-id byte (with time out beacuse it may not arrive)
    if (ps2_wait_for_read_with_timeout() == 0) {
        ps2_state.first_port_device_id[1] = inb(PS2_DATA_PORT);
    }

    // send Enable-Scanning command
    if (ps2_send_to_first_port(0xf4) != 0) {
        // timed out
        ps2_state.first_port_working = 0;
        ps2_state.first_port_timed_out = 1;
        return;
    }
    ps2_wait_for_ack();
}

void ps2_get_second_port_device_id() {
    uint8_t response;

    ps2_state.second_port_device_id[0] = 0x00;
    ps2_state.second_port_device_id[1] = 0x00;

    // send Disable-Scanning command
    if (ps2_send_to_second_port(0xf5) != 0) {
        // timed out
        ps2_state.second_port_working = 0;
        ps2_state.second_port_timed_out = 1;
        return;
    }
    // wait for ACK response
    ps2_wait_for_ack();
    
    // send Identity command
    if (ps2_send_to_second_port(0xf2) != 0) {
        // timed out
        ps2_state.second_port_working = 0;
        ps2_state.second_port_timed_out = 1;
        return;
    }
    // wait for ACK response
    ps2_wait_for_ack();

    // get first device-id byte
    ps2_wait_for_read();
    ps2_state.second_port_device_id[0] = inb(PS2_DATA_PORT);

    // get second device-id byte (with time out beacuse it may not arrive)
    if (ps2_wait_for_read_with_timeout() == 0) {
        ps2_state.second_port_device_id[1] = inb(PS2_DATA_PORT);
    }

    // send Enable-Scanning command
    if (ps2_send_to_second_port(0xf4) != 0) {
        // timed out
        ps2_state.second_port_working = 0;
        ps2_state.second_port_timed_out = 1;
        return;
    }
    ps2_wait_for_ack();
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

    // reset devices
    if (ps2_state.first_port_working == 1) {
        ps2_reset_first_port();
    }
    if (ps2_state.second_port_working == 1) {
        ps2_reset_second_port();
    }
    // if both ports dont work, give up
    if (ps2_state.first_port_working == 0 && ps2_state.second_port_working == 0) {
        // error
        return 3;
    }

    // get device ids
    if (ps2_state.first_port_working == 1) {
        ps2_get_first_port_device_id();
    }
    if (ps2_state.second_port_working == 1) {
        ps2_get_second_port_device_id();
    }
    // if both ports dont work, give up
    if (ps2_state.first_port_working == 0 && ps2_state.second_port_working == 0) {
        // error
        return 4;
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

    // init success
    return 0;
}
