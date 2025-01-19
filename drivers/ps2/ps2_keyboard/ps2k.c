#include "ps2k.h"
#include "../ps2.h"
#include "../../../kernel/text_mode.h"


#define PS2K_COMMAND_QUEUE_MAX_SIZE 16
#define PS2K_COMMAND_MAX_ARGS 2
#define PS2K_COMMAND_MAX_RESPONSE 2


enum ps2_command_type {
    PS2K_ECHO = 0
};

struct ps2k_command {
    enum ps2_command_type cmd;
    uint8_t args[PS2K_COMMAND_MAX_ARGS];
};

struct ps2k_device {
    struct ps2k_command command_queue[PS2K_COMMAND_QUEUE_MAX_SIZE];
    uint8_t command_queue_length;
    uint8_t waiting_for_response : 1;   // also means that scanning is disabled
    uint8_t current_response_length;
    uint8_t response[PS2K_COMMAND_MAX_RESPONSE];
};


// there are only 2 ps2 devices possible (0 or 1)
static struct ps2k_device ps2k_devices[2];

// assumes that:
// -    devc==&(ps2k_devices[device])
// -    cmd0==&(devc->command_queue[0])
// -    devc->waiting_for_resonse==0 (or that resend code received)
static void ps2k_send_echo_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0) {
    devc->waiting_for_response = 1;
    devc->current_response_length = 0;
    ps2_send_byte(device, 0xee);
}
static void ps2k_echo_cmd_handle_receive() {

}

// sends the first command in the device's queue
static void ps2k_send_command(uint8_t device) {
    if (device > 1) return;
    
    struct ps2k_device* devc = &(ps2k_devices[device]);
    
    // dont run over current command
    if (devc->waiting_for_response == 1) return;

    // dont send non-existing command
    if (devc->command_queue_length == 0) return;

    struct ps2k_command* cmd0 = &(devc->command_queue[0]);

    switch (cmd0->cmd) {
        case PS2K_ECHO:
            ps2k_send_echo_cmd(device, devc, cmd0);
            break;
    }
}

static void ps2k_queue_command(uint8_t device, struct ps2k_command cmd) {
    if (device > 1) return;

    struct ps2k_device* devc = &(ps2k_devices[device]);
    
    devc->command_queue[devc->command_queue_length] = cmd;
    devc->command_queue_length++;

    if (devc->command_queue_length == 1) {
        ps2k_send_command(device);
    }
}

static void ps2k_device_send_echo(uint8_t device) {
    ps2k_queue_command(device, (struct ps2k_command){.cmd = PS2K_ECHO});
}

static uint8_t ps2k_device_irq_handler(uint8_t device, uint8_t data) {
    // device is always 0 or 1
    struct ps2k_device* devc = &(ps2k_devices[device]);
    // check for a command waiting for response
    if (devc->command_queue_length != 0 && devc->waiting_for_response == 1) {
        // command waiting for response
        struct ps2k_command* cmd0 = &(devc->command_queue[0]);
        

    }
    
    print_char('\n', 0);
    print_hex8(data, TM_FORE_COL_BROWN);
    return 0;
}

static struct ps2_device_driver ps2k_device_driver = (struct ps2_device_driver) {
    .long_device_id = 0,
    .device_id = {0xab, 0x00},
    .send_echo = &ps2k_device_send_echo,
    .irq_handler = &ps2k_device_irq_handler
};

void ps2_keyboard_device_driver_install() {
    ps2_install_device_driver(&ps2k_device_driver);
}
