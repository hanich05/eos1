#pragma once

#include "isr.h"
#include "../gdt.h"
#include "idt.h"
#include "../../drivers/pic/pic.h"
#include "../text_mode.h"
#include <stdint.h>


__attribute__((packed))
struct registers_t {
    // old data segment
    uint32_t ds;
    // pushad
    uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
    // pushed by isr-stub
    uint32_t interrupt, error;
    // pushed by the CPU
    uint32_t eip, cs, eflags, esp, ss
};


extern void (*isr_stubs_table[256])(void);

void isrs_intall() {
    pic_remap(IRQ0, IRQ8);
    for (uint32_t i = 0; i < 256; i++) {
        idt_set_gate(i, CODE_SEG, isr_stubs_table[i], IDT_FLAG_DPL_RING0 | IDT_FLAG_GATE_32INT | IDT_FLAG_PRESENT);
    }
    idt_load();
    asm("sti");
}

/// @brief Convert an interrupt number to an IRQ number
/// @return -1 if not an IRQ, otherwise the IRQ number.
static int8_t int_to_irq(uint8_t interrupt) {
    if (interrupt >= IRQ0 && interrupt <= IRQ7) {   // master PIC - IRQs
        return interrupt-IRQ0;
    }
    if (interrupt >= IRQ8 && interrupt <= IRQ15) {  // slave PIC - IRQs
        return interrupt-IRQ8;
    }
    return -1;
}

void interrupt_handler(struct registers_t* regs) {
    int8_t irq = int_to_irq(regs->interrupt);
    
    print_string("\nInterrupt ", TM_FORE_COL_GRAY);
    print_hex8(regs->interrupt, TM_FORE_COL_GREEN);

    if (irq != -1) {
        print_string(" (IRQ ", TM_FORE_COL_GRAY);
        print_hex8((uint8_t)irq, TM_FORE_COL_GREEN);
        print_string(")", TM_FORE_COL_GRAY);    
    }
    print_string(" handled", TM_FORE_COL_GRAY);
    
    if (regs->error != 0x0000) {
        do {
            print_char(' ', 0);
        }while (curs_pos.x % 8 != 0);
        print_string("error is ", TM_FORE_COL_GRAY);
        print_hex8(regs->error, TM_FORE_COL_RED);
    }
    
    if (irq != -1) {
        pic_send_eoi((uint8_t)irq);
    }
}
