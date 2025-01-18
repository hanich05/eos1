#pragma once

#include "../isrs.h"
#include <stdint.h>

#define IRQ0    32
#define IRQ1    (IRQ0+1)
#define IRQ2    (IRQ0+2)
#define IRQ3    (IRQ0+3)
#define IRQ4    (IRQ0+4)
#define IRQ5    (IRQ0+5)
#define IRQ6    (IRQ0+6)
#define IRQ7    (IRQ0+7)
#define IRQ8    40
#define IRQ9    (IRQ8+1)
#define IRQ10   (IRQ8+2)
#define IRQ11   (IRQ8+3)
#define IRQ12   (IRQ8+4)
#define IRQ13   (IRQ8+5)
#define IRQ14   (IRQ8+6)
#define IRQ15   (IRQ8+7)


/// @brief Convert an interrupt number to an IRQ number
/// @return -1 if not an IRQ, otherwise the IRQ number.
int8_t int_to_irq(uint8_t interrupt);
uint8_t irq_handler(uint8_t irq, struct registers_t* regs);
void irqs_install();