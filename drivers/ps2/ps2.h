#pragma once

#include <stdint.h>


#define PS2_DEFAULT_READ_TIMEOUT 2
#define PS2_DEFAULT_WRITE_TIMEOUT 2
#define PS2_DEFAULT_ECHO_INTERVAL 20
#define PS2_MAX_DEVICE_DRIVERS 256


struct ps2_device_driver {
    uint8_t long_device_id  : 1;
    uint8_t device_id[2];
    // when the device echos back the handler should call 
    void (*send_echo)(uint8_t device);
    // shoult return 1 on time-out or 0 on success
    uint8_t (*irq_handler)(uint8_t device, uint8_t data);
};


uint8_t ps2_init();

/// @brief Set the driver that will handle ps2 devices with his device-id.
///     If the device-id is short (1 byte) then any ps2 deivces with a long device-id that has the same
///     first byte and no dedicated device driver will use this device driver.
///     If there were already PS2_MAX_DEVICE_DRIVERS devices intalled the device will be ignored.
/// @param driver A pointer to a ps2_device_driver struct representing the driver.
///     The device driver must implement a send_echo functionality and an irq_handler.
void ps2_install_device_driver(struct ps2_device_driver* driver);

void ps2_set_read_timeout(uint32_t to);
void ps2_set_write_timeout(uint32_t to);
uint32_t ps2_get_read_timeout();
uint32_t ps2_get_write_timeout();

/// @brief Send a byte to a connected PS2 device
/// @param device The device number (0 or 1)
/// @param byte The byte that will be sent
/// @returns 2 if device does not exists, 1 on time-out, otherwise 0
uint8_t ps2_send_byte(uint8_t device, uint8_t byte);
/// @brief Receive a byte from the PS2's input buffer
/// @param byte Where the received byte will be stored
/// @returns 1 on time-out, otherwise 0
uint8_t ps2_receive_byte(uint8_t* byte);

void ps2_enable_port(uint8_t port);
void ps2_disable_port(uint8_t port);
/// @brief 
/// @param device 
/// @return 2 if port does not exists (not 1 or 0), 1 if failed, 0 if passed
uint8_t ps2_test_port(uint8_t port);

/// @param device The device number (0 or 1)
/// @returns 2 if device does not exist, 1 on time-out, otherwise 0
uint8_t ps2_diable_scanning(uint8_t device);
/// @param device The device number (0 or 1)
/// @returns 2 if device does not exist, 1 on time-out, otherwise 0
uint8_t ps2_enable_scanning(uint8_t device);

/// @brief Should be called by ps2 device drivers when an echo sent by it's send_echo sent a response
/// @param device The device number (0 or 1)
void ps2_declare_echo_success(uint8_t device);

// ps2 first device
void irq1_handler();
// ps2 second device
void irq12_handler();