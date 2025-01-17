#pragma once

#include <stdint.h>

#define IDT_FLAG_GATE_TASK      0x05
#define IDT_FLAG_GATE_16INT     0x06
#define IDT_FLAG_GATE_16TRAP    0x07
#define IDT_FLAG_GATE_32INT     0x0e
#define IDT_FLAG_GATE_32TRAP    0x0f

#define IDT_FLAG_DPL_RING0      0b00000000
#define IDT_FLAG_DPL_RING1      0b00100000
#define IDT_FLAG_DPL_RING2      0b01000000
#define IDT_FLAG_DPL_RING3      0b01100000

#define IDT_FLAG_PRESENT        0b10000000
#define IDT_FLAG_NOT_PRESENT    0b00000000

extern uint32_t idt[256];

__attribute__((sysv_abi))
void idt_set_gate(uint8_t interrupt, uint16_t segment_selector, void (*base)(void), uint8_t flags);

/// @brief Calls lidt with a idt descriptor that points to the idt. This function must be called at least once for interrupts to work.
__attribute__((sysv_abi))
void idt_load();