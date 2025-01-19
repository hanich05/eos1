#pragma once

#include <stdint.h>


#define PS2_MAX_DEVICE_DRIVERS 256


struct ps2_device_driver {
    uint8_t long_device_id  : 1;
    uint8_t device_id[2];
    void (*send_echo)(uint8_t device);
    uint8_t (*irq_handler)(uint8_t device, uint8_t data);
};


/// @brief Set the driver that will handle ps2 devices with his device-id.
///     If there were already PS2_MAX_DEVICE_DRIVERS devices intalled the device will be ignored.
/// @param driver A pointer to a ps2_device_driver struct representing the driver.
///     The device driver must implement a send_echo functionality and an irq_handler.
void ps2_install_device_driver(struct ps2_device_driver* driver);

// ps2 first device
void irq1_handler();

// ps2 second device
void irq12_handler();

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

void ps2_enable_device(uint8_t device);
void ps2_disable_device(uint8_t device);
/// @brief 
/// @param device 
/// @return 2 if device does not exists, 1 if failed, 0 if passed
uint8_t ps2_test_device(uint8_t device);

// implemented in 'ps2.h'
uint8_t ps2_init();
