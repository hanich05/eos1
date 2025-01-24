#include "text_mode.h"
#include "interrupts/isrs.h"
#include "../drivers/ps2/ps2.h"
#include "../drivers/ps2/ps2_keyboard/ps2k.h"

void kmain() {
    text_mode_init();
    clear_screen();

    print_string("Kernel initialized successfully", TM_FORE_COL_GRAY);

    isrs_intall();
    print_string("\nInterrupts initialized successfully", TM_FORE_COL_GRAY);
    
    uint8_t ps2_init_val = ps2_init();
    if (ps2_init_val != 0) {
        print_string("\nps2_init() failed with error code ", TM_FORE_COL_RED);
        print_hex8(ps2_init_val, TM_FORE_COL_BLUE);
    }

    ps2_keyboard_device_driver_install();

    return;
}