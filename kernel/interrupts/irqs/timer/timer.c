#include "timer.h"
#include <stdint.h>

uint32_t ticks_since_boot;

void irq0_handler() {
    ticks_since_boot++;
}
