#include "isrs.h"
#include "../gdt.h"
#include "idt.h"
#include "irqs/irqs.h"
#include "../text_mode.h"
#include <stdint.h>


extern void (*isr_stubs_table[256])(void);

void isrs_intall() {
    irqs_install();
    for (uint32_t i = 0; i < 256; i++) {
        idt_set_gate(i, CODE_SEG, isr_stubs_table[i], IDT_FLAG_DPL_RING0 | IDT_FLAG_GATE_32INT | IDT_FLAG_PRESENT);
    }
    idt_load();
    asm("sti");
}

static uint8_t exception_handler(struct registers_t* regs) {

}

// called by 'isr_common' in 'isr_stubs.s'
void interrupt_handler(struct registers_t* regs) {
    int8_t irq = int_to_irq(regs->interrupt);
    uint8_t handled = 0;
    
    asm("cli");
    
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
