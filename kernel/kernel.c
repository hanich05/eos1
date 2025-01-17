#include "text_mode.h"
#include "interrupts/isr.h"

void kmain() {
    text_mode_init();
    clear_screen();

    print_string("Kernel initialised successfully", TM_FORE_COL_GRAY);

    isrs_intall();

    // print_char('\n', 0);
    // print_hex32((uint32_t)&int_handler, TM_FORE_COL_GREEN);

    // idt_set_gate(49, CODE_SEG, &int_handler, IDT_FLAG_DPL_RING0 | IDT_FLAG_GATE_32INT | IDT_FLAG_PRESENT);
    
    // idt_load();
    
    // print_memory_block((uint32_t)idt+0x100, (uint32_t)idt+0x200);

    return;
}