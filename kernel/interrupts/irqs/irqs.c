#include "irqs.h"
#include "../../../libc/io.h"
#include "../../../drivers/pic/pic.h"
#include "timer/timer.h"
#include "../../../drivers/ps2/ps2.h"
#include "../../text_mode.h"
#include <stdint.h>


int8_t int_to_irq(uint8_t interrupt) {
    if (interrupt >= IRQ0 && interrupt <= IRQ7) {   // master PIC - IRQs
        return interrupt-IRQ0;
    }
    if (interrupt >= IRQ8 && interrupt <= IRQ15) {  // slave PIC - IRQs
        return interrupt-IRQ8;
    }
    return -1;
}

static uint8_t handle_if_spurious(uint8_t irq) {
    uint16_t pic_isr = pic_get_isr();

    // check if IRQ is a master-PIC Spurious IRQ
    if (irq == 7) {
        // if ISR does not indicate that the IRQ is being serviced then it is a Spurious IRQ.
        if (pic_isr & 0x0080 == 0) {
            // the IRQ is a Spurious IRQ
            return 1;
        }
    }
    // check if IRQ is a slave-PIC Spurious IRQ
    if (irq == 15) {
        // if ISR does not indicate that the IRQ is being serviced then it is a Spurious IRQ.
        if (pic_isr & 0x8000 == 0) {
            // the IRQ is a Spurious IRQ
            // send an EOI only to the master-PIC that thinks this IRQ is legitimate
            pic_send_eoi(7);
            return 1;
        }
    }

    return 0;
}

uint8_t irq_handler(uint8_t irq, struct registers_t* regs) {
    uint8_t handled = 0;

    if (handle_if_spurious(irq) == 1) {
        handled = 1;
        return handled;
    }

    switch (irq) {
        case 0: // timer (every 55ms)
            irq0_handler();
            handled = 1;
            break;
        case 1: { // ps2 first port interrupt
            irq1_handler();
            handled = 1;
            break;
        }
        case 4:{ // COM1 (mouse?)
            print_string("\nMouse?", TM_FORE_COL_GRAY);
            break;
        }
        case 12: { // ps2 second port interrupt
            irq12_handler();
            handled = 1;
            break;
        }
    }

    pic_send_eoi((uint8_t)irq);

    return handled;
}

void irqs_install() {
    pic_remap(IRQ0, IRQ8);
}