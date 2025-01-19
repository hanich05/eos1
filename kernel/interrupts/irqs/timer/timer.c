#include "timer.h"
#include <stdint.h>

uint32_t ticks_since_boot;

struct intervaled_callback {
    void (*func)(void);
    uint8_t interval;
    uint8_t offset;
};

static struct intervaled_callback intervaled_callbacks[MAX_INTERVALED_CALLBACKS];
static uint32_t intervaled_callbacks_length;

uint32_t install_intervaled_callback(void (*func)(void), uint8_t interval, uint8_t offset) {
    // don't overflow
    if (intervaled_callbacks_length >= MAX_INTERVALED_CALLBACKS) return 0;

    // set entry
    intervaled_callbacks[intervaled_callbacks_length] = (struct intervaled_callback) {
        .func = func,
        .interval = interval,
        .offset = offset
    };
    intervaled_callbacks_length++;
    return intervaled_callbacks_length; // entry_position+1
}

void intervaled_callback_update_interval(uint32_t uid, uint8_t interval) {
    // don't access out of bounds enries. shouldn't happen anyway
    if (uid-1 >= MAX_INTERVALED_CALLBACKS || uid == 0) return;

    intervaled_callbacks[uid-1].interval = interval;
}

void irq0_handler() {
    ticks_since_boot++;

    // call intervaled callbacks
    for (uint32_t i = 0; i < intervaled_callbacks_length; i++) {
        if (ticks_since_boot % intervaled_callbacks[i].interval == intervaled_callbacks[i].offset) {
            intervaled_callbacks[i].func();
        }
    }
}
