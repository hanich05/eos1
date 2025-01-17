#pragma once

// from: https://wiki.osdev.org/8259_PIC#Articles

#include "pic.h"
#include "../../libc/io.h"
#include <stdint.h>

#define PIC1                0x20
#define PIC2                0xa0
#define PIC1_COMMAND        0x20
#define PIC1_DATA           0x21
#define PIC2_COMMAND        0xa0
#define PIC2_DATA           0xa1

// PIC End Of Interrupt command
#define PIC_ICW1_EOI        0x20
// PIC Init command
// Should be followed by 3 words for the PIC:
// - its vector offset
// - tell it how it is wired to master/slaves
// - gives additional information about the enviorment
#define PIC_ICW1_INIT       0x11
// indicate that ICW4 will be present
#define PIC_ICW1_ICW4       0x01
// 8086/88 (MCS-80/85) mode
#define ICW4_8086	        0x01

// PIC irq ready next command
#define PIC_OCW3_READ_IRR   0x0a
// PIC irq service next command
#define PIC_OCW3_READ_ISR   0x0b


void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        // send eoi to slave-pic if IRQ was from slave-pic
        outb(PIC2_COMMAND, PIC_ICW1_EOI);
    }

    // send eoi to master-pic no matter what
    outb(PIC1_COMMAND, PIC_ICW1_EOI);
}

void pic_remap(uint8_t offset1, uint8_t offset2) {
    uint8_t a1, a2;

    // save masks
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    // start the initialization sequence (in cascade mode)
    outb(PIC1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    outb(PIC2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    // ICW2: master PIC vector offset
    outb(PIC1_DATA, offset1);
    // ICW2: slave PIC vector offset
    outb(PIC2_DATA, offset2);
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (00000100)
	outb(PIC1_DATA, 4);
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb(PIC2_DATA, 2);
    
    // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

    // restore saved masks.
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void pic_disable() {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void irq_set_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

void irq_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

static uint16_t __pic_get_irq_reg(uint8_t ocw3) {
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

uint16_t pic_get_irr() {
    return __pic_get_irq_reg(PIC_OCW3_READ_IRR);
}

uint16_t pic_get_isr() {
    return __pic_get_irq_reg(PIC_OCW3_READ_ISR);
}