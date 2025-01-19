#include "ps2k.h"
#include "../ps2.h"
#include "../../../kernel/text_mode.h"

static void ps2_keyboard_device_send_echo(uint8_t device) {
    ps2_send_byte(device, 0xee);
}

static uint8_t ps2_keyboard_device_irq_handler(uint8_t device, uint8_t data) {
    print_char('\n', 0);
    print_hex8(data, TM_FORE_COL_BROWN);
    return 0;
}

static struct ps2_device_driver ps2_keyboard_device_driver = (struct ps2_device_driver) {
    .long_device_id = 0,
    .device_id = {0xab, 0x00},
    .send_echo = &ps2_keyboard_device_send_echo,
    .irq_handler = &ps2_keyboard_device_irq_handler
};

void ps2_keyboard_device_driver_install() {
    ps2_install_device_driver(&ps2_keyboard_device_driver);
}
