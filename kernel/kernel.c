#include "text_mode.h"
#include "interrupts/isrs.h"
#include "../drivers/ps2/ps2.h"

void kmain() {
    text_mode_init();
    clear_screen();

    print_string("Kernel initialized successfully", TM_FORE_COL_GRAY);

    isrs_intall();
    print_string("\nInterrupts initialized successfully", TM_FORE_COL_GRAY);
    
    uint8_t ps2_init_val = ps2_init();
    print_char('\n', 0);
    print_hex8(ps2_init_val, TM_FORE_COL_BLUE);
    print_char('\n', 0);
    print_bin8(*((uint8_t*)&ps2_state), TM_FORE_COL_MAGENTA);

    // print_char('\n', 0);
    // print_hex32((uint32_t)&int_handler, TM_FORE_COL_GREEN);

    // idt_set_gate(49, CODE_SEG, &int_handler, IDT_FLAG_DPL_RING0 | IDT_FLAG_GATE_32INT | IDT_FLAG_PRESENT);
    
    // idt_load();
    
    // print_memory_block((uint32_t)idt+0x100, (uint32_t)idt+0x200);

    return;
}