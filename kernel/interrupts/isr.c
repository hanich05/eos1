#pragma once

#include "isr.h"
#include "../gdt.h"
#include "idt.h"
#include "../../libc/io.h"
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

static uint8_t exception_handler(struct registers_t* regs) {

}

static uint8_t irq_handler(uint8_t irq, struct registers_t* regs) {
    uint16_t pic_isr = pic_get_isr();

    // check if IRQ is a master-PIC Spurious IRQ
    if (irq == 7) {
        // if ISR does not indicate that the IRQ is being serviced then it is a Spurious IRQ.
        if (pic_isr & 0x0080 == 0) {
            // the IRQ is a Spurious IRQ
            asm("sti");
            return 0;
        }
    }
    // check if IRQ is a slave-PIC Spurious IRQ
    if (irq == 15) {
        // if ISR does not indicate that the IRQ is being serviced then it is a Spurious IRQ.
        if (pic_isr & 0x8000 == 0) {
            // the IRQ is a Spurious IRQ
            // send an EOI only to the master-PIC that thinks this IRQ is legitimate
            pic_send_eoi(7);
            
            asm("sti");
            return 0;
        }
    }

    uint8_t handled = 0;

    switch (irq) {
        case 0: // timer (every 55ms)
            handled = 1;
            break;
        case 1: // keyboard interrupt
            uint8_t val = inb(0x60);
            print_char('\n', 0);
            print_hex8(val, TM_FORE_COL_BROWN);
            handled = 1;
            break;
    }

    pic_send_eoi((uint8_t)irq);

    return handled;
}

// called by 'isr_common' in 'isr_stubs.s'
void interrupt_handler(struct registers_t* regs) {
    asm("cli");
    int8_t irq = int_to_irq(regs->interrupt);
    
    uint8_t handled = 0;
    
    if (regs->interrupt < 32) {
        handled = exception_handler(regs);
    }
    else
    if (irq != -1) {
        handled = irq_handler((uint8_t)irq, regs);
    }
    
    if (handled == 0) {
        print_string("\nInterrupt ", TM_FORE_COL_GRAY);
        print_hex8(regs->interrupt, TM_FORE_COL_GREEN);

        if (irq != -1) {
            print_string(" (IRQ ", TM_FORE_COL_GRAY);
            print_hex8((uint8_t)irq, TM_FORE_COL_GREEN);
            print_string(")", TM_FORE_COL_GRAY);    
        }
        print_string(" not handled", TM_FORE_COL_GRAY);
        
        if (regs->error != 0x0000) {
            do {
                print_char(' ', 0);
            }while (curs_pos.x % 8 != 0);
            print_string("; error is ", TM_FORE_COL_GRAY);
            print_hex8(regs->error, TM_FORE_COL_RED);
        }
    }

    asm("sti");
}
