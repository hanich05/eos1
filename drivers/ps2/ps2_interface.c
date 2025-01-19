#include "ps2_interface.h"
#include "ps2.h"
#include "../../libc/io.h"


static struct ps2_device_driver* ps2_device_drivers[PS2_MAX_DEVICE_DRIVERS];
static uint16_t ps2_device_drivers_amount = 0;
static uint16_t ps2_first_device_driver   = 0; // ps2_device_drivers_amount if not set yet
static uint16_t ps2_second_device_driver  = 0; // ps2_device_drivers_amount if not set yet


void ps2_install_device_driver(struct ps2_device_driver* driver) {
    if (ps2_device_drivers_amount >= PS2_MAX_DEVICE_DRIVERS) return;
    
    ps2_device_drivers[ps2_device_drivers_amount] = driver;

    if (ps2_first_device_driver == ps2_device_drivers_amount) ps2_first_device_driver++;
    if (ps2_second_device_driver == ps2_device_drivers_amount) ps2_second_device_driver++;
    
    ps2_device_drivers_amount++;
}

static uint16_t device_find_driver(uint8_t long_device_id, uint8_t* device_id) {
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
    }else {
        for (uint16_t i = 0; i < ps2_device_drivers_amount; i++) {
            if (ps2_device_drivers[i]->device_id[0] == device_id[0]) {
                return i;
            }
        }
    }
    return ps2_device_drivers_amount;
}

// ps2 first device
void irq1_handler() {
    uint8_t data = inb(PS2_DATA_PORT);
    if (ps2_first_device_driver == ps2_device_drivers_amount) {
        ps2_first_device_driver = device_find_driver(ps2_state.first_port_long_device_id, ps2_state.first_port_device_id);
        if (ps2_first_device_driver == ps2_device_drivers_amount) return;
    }
    ps2_device_drivers[ps2_first_device_driver]->irq_handler(0, data);
}

// ps2 second device
void irq12_handler() {
    uint8_t data = inb(PS2_DATA_PORT);
    if (ps2_second_device_driver == ps2_device_drivers_amount) {
        ps2_second_device_driver = device_find_driver(ps2_state.second_port_long_device_id, ps2_state.second_port_device_id);
        if (ps2_second_device_driver == ps2_device_drivers_amount) return;
    }
    ps2_device_drivers[ps2_second_device_driver]->irq_handler(0, data);
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
    if (device == 0) {
        return ps2_send_to_first_port(byte);
    }
    if (device == 1) {
        return ps2_send_to_second_port(byte);
    }
    // device does not exist
    return 2;
}
uint8_t ps2_receive_byte(uint8_t* byte) {
    if (ps2_wait_for_read_with_timeout() == 1) {
        // timed out
        return 1;
    }
    *byte = inb(PS2_DATA_PORT);
    return 0;
}

void ps2_enable_device(uint8_t device) {
    if (device == 0) {
        ps2_enable_first_ps2_port();
    }
    if (device == 1) {
        ps2_enable_second_ps2_port();
    }
}
void ps2_disable_device(uint8_t device) {
    if (device == 0) {
        ps2_disable_first_ps2_port();
    }
    if (device == 1) {
        ps2_disable_second_ps2_port();
    }
}
uint8_t ps2_test_device(uint8_t device) {
    if (device == 0) {
        return (ps2_test_first_ps2_port() != 0);
    }
    if (device == 1) {
        return (ps2_test_second_ps2_port() != 0);
    }
    // device does not exist
    return 2;
}
