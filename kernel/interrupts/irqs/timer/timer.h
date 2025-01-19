#pragma once

#include <stdint.h>

#define MAX_INTERVALED_CALLBACKS 256

// every tick is about 55ms
extern uint32_t ticks_since_boot;

/// @brief Set an interval callback to a function.
/// @param func The function that will be called.
/// @param interval The interval (in ticks - 55ms) between calls
/// @param offset An offset (in ticks - 55ms) for the interval. Use diffrent offsets for diffrent callbacks to ensure a spread of callbacking.
/// @return 0 on error, otherwise a unique identifier for this intervaled-callback.
uint32_t install_intervaled_callback(void (*func)(void), uint8_t interval, uint8_t offset);

/// @brief Update the interval for an installed intervaled-callback.
/// @param uid The unique identifier of an installed intervaled-callback.
/// @param interval The new interval (in ticks - 55ms) between calls.
void intervaled_callback_update_interval(uint32_t uid, uint8_t interval);

// Programmable Interrupt Timer Interrupt handler
void irq0_handler();