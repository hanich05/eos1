#include "text_mode.h"

void kmain() {
    text_mode_init();
    clear_screen();

    print_string("Kernel initialised successfully", TM_FORE_COL_WHITE);
    
    return;
}