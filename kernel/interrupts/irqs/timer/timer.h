#pragma once

#include <stdint.h>

// every tick is about 55ms
extern uint32_t ticks_since_boot;

// Programmable Interrupt Timer Interrupt handler
void irq0_handler();