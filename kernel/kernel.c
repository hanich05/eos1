#include "text_mode.h"
#include "../libc/memmove.h"

void kmain() {
    text_mode_init();
    clear_screen();

    print_string("Kernel initialised successfully", TM_FORE_COL_WHITE);
    
    return;
}