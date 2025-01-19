#pragma once

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

struct ps2_state {
    uint8_t controller_working          : 1;
    uint8_t is_dual                     : 1;
    uint8_t first_port_working          : 1;
    uint8_t second_port_working         : 1;
    uint8_t first_port_timed_out        : 1;
    uint8_t second_port_timed_out       : 1;

    uint8_t first_port_long_device_id   : 1;
    uint8_t second_port_long_device_id  : 1;
    uint8_t first_port_device_id[2];
    uint8_t second_port_device_id[2];

    uint8_t first_port_reset_response[2];
    uint8_t second_port_reset_response[2];
};


extern uint32_t ps2_read_timeout;
extern uint32_t ps2_write_timeout;
extern struct ps2_state ps2_state;


struct ps2_status_reg ps2_read_status_reg();
struct ps2_controller_config ps2_read_controller_config();
struct ps2_controller_output_port ps2_read_controller_output_port();
void ps2_write_controller_config(struct ps2_controller_config cc);
void ps2_write_to_controller_output_port(struct ps2_controller_output_port cop);

/// @brief stops code execution until PS2's output-buffer-status (bit0) is set
void ps2_wait_for_read();
/// @brief stops code execution until PS2's output-buffer-status (bit0) is set.
/// Times out after ps2_read_timeout ticks
/// @returns 1 if timed-out, otherwise 0
uint8_t ps2_wait_for_read_with_timeout();
/// @brief stops code execution until PS2's output-buffer-status (bit0) is empty
void ps2_wait_for_empty_output_buffer();
/// @brief stops code execution until PS2's input-buffer-status (bit1) is clear
void ps2_wait_for_write();
/// @brief stops code execution until PS2's input-buffer-status (bit1) is clear.
/// Times out after ps2_write_timeout ticks
/// @returns 1 if timed-out, otherwise 0
uint8_t ps2_wait_for_write_with_timeout();

void ps2_send_cmd(uint8_t cmd);
void ps2_send_cmd_arg(uint8_t cmd, uint8_t arg);
uint8_t ps2_responsive_send_cmd(uint8_t cmd);

uint8_t ps2_ram_read_byte_0();
uint8_t ps2_ram_read_byte_n(uint8_t n);
void ps2_ram_write_byte_0(uint8_t byte);
void ps2_ram_write_byte_n(uint8_t n, uint8_t byte);

// only if 2 PS2 ports supported
void ps2_disable_first_ps2_port();
void ps2_disable_second_ps2_port();
// only if 2 PS2 ports supported
void ps2_enable_first_ps2_port();
void ps2_enable_second_ps2_port();

/// @returns 0x55 = test passed ; 0xfc = test failed
uint8_t ps2_test_ps2_contoller();
/// @returns 0x00 = test passed ; 0x01 = clock line stuck low ; 0x02 = clock line stuck high ; 0x03 = data line stuck low ; 0x04 = data line stuck high
uint8_t ps2_test_first_ps2_port();
/// only if 2 PS2 ports supported
/// @returns 0x00 = test passed ; 0x01 = clock line stuck low ; 0x02 = clock line stuck high ; 0x03 = data line stuck low ; 0x04 = data line stuck high
uint8_t ps2_test_second_ps2_port();

/// @return unkown (none of the bits have a standard/defined purpose)
uint8_t ps2_read_controller_input_port();
void ps2_copy_input_port_low_to_status_high();
void ps2_copy_input_port_high_to_status_high();

/// pulsed lines low for 6ms according to mask.
/// bit 0 corresponds to the "reset" line. the other output lines don't have a standard/defined purpose
/// @param mask a 4 bit mask (0 = pulse line, 1 = don't pulse line). correspond to 4 different output lines
void pulse_output_lines_low(uint8_t mask);

/// only if first port is working
/// @returns 1 if timed-out, otherwise 0
uint8_t ps2_send_to_first_port(uint8_t byte);
/// only if 2 PS2 ports supported
/// only if second port is working
/// @returns 1 if timed-out, otherwise 0
uint8_t ps2_send_to_second_port(uint8_t byte);

/// waits for 0xfa to arrive in PS2_DATA_PORT
void ps2_wait_for_ack();

// assumes both ports' interrupts are disabled
// saves response in ps2_state.first_port_reset_response
void ps2_reset_first_port();
// assumes both ports' interrupts are disabled
// saves response in ps2_state.second_port_reset_response
void ps2_reset_second_port();
// assumes both ports' interrupts are disabled
// saves response in ps2_state.first_port_device_id
void ps2_get_first_port_device_id();
// assumes both ports' interrupts are disabled
// saves response in ps2_state.second_port_device_id
void ps2_get_second_port_device_id();

uint8_t ps2_init();