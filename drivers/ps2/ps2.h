#pragma once

#include <stdint.h>

struct ps2_state {
    uint8_t controller_working  : 1;
    uint8_t is_dual             : 1;
    uint8_t first_port_working  : 1;
    uint8_t second_port_working : 1;
};

extern struct ps2_state ps2_state;

uint8_t ps2_init();