#pragma once

#include <stdint.h>

void pic_send_eoi(uint8_t irq);
/// @param offset1 vector offset for master PIC. vectors on the master become offset1..offset1+7
/// @param offset2 same for slave PIC: offset2..offset2+7
void pic_remap(uint8_t offset1, uint8_t offset2);
void pic_disable();
void irq_set_mask(uint8_t irq_line);
void irq_clear_mask(uint8_t irq_line);
/// @returns The combined value of the cascaded PICs irq request register
uint16_t pic_get_irr();
/// @returns The combined value of the cascaded PICs in-service register
uint16_t pic_get_isr();