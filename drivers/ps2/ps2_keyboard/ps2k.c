#include "ps2k.h"
#include "../ps2.h"
#include "../../../libc/memmove.h"
#include "../../../kernel/text_mode.h"


/*
    BUG: when disabling/enabling scanning. the device sends a 0xfa (ACK) response which does not get handled yet.
*/


#define PS2K_COMMAND_QUEUE_MAX_SIZE 16
#define PS2K_COMMAND_MAX_ARGS 2
#define PS2K_COMMAND_MAX_RESPONSE 2


enum ps2_command_type {
    PS2K_ECHO = 0,

    PS2K_COMMAND_TYPES_AMOUNT
};

struct ps2k_command {
    enum ps2_command_type cmd;
    uint8_t args[PS2K_COMMAND_MAX_ARGS];
};

struct ps2k_device {
    uint8_t scanning_enabled_before_disable     : 1;
    uint8_t scanning_enabled                    : 1;
    uint8_t waiting_for_enable_scanning_ack     : 1;
    uint8_t waiting_for_disable_scanning_ack    : 1;
    uint8_t waiting_for_response                : 1; // Should also mean that scanning is disabled
    
    uint8_t command_queue_length;
    struct ps2k_command command_queue[PS2K_COMMAND_QUEUE_MAX_SIZE];
    
    uint8_t current_response_length;
    uint8_t response[PS2K_COMMAND_MAX_RESPONSE];
};

struct ps2k_command_handler {
    // can assume that:
    // -    scanning is disabled
    // -    devc==&(ps2k_devices[device])
    // -    cmd0==&(devc->command_queue[0])
    // -    devc->waiting_for_resonse==0 (or that resend code received and handler recalled)
    void (*send)(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0);
    uint8_t (*handle_receive)(uint8_t device, struct ps2k_device* devc, uint8_t data);
};



// should be called when the current command has resolved all responses
static void ps2k_command_queue_pop(uint8_t device, struct ps2k_device* devc);
static void ps2k_queue_command(uint8_t device, struct ps2k_command cmd);
// sends the first command in the device's queue
static void ps2k_send_command(uint8_t device, struct ps2k_device* devc);

static void ps2k_send_echo_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0);
static uint8_t ps2k_echo_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data);


// there are only 2 ps2 devices possible (0 or 1)
static struct ps2k_device ps2k_devices[2];
static struct ps2k_command_handler ps2k_command_handlers[PS2K_COMMAND_TYPES_AMOUNT] = {
    [PS2K_ECHO] = (struct ps2k_command_handler) {
        .send = &ps2k_send_echo_cmd,
        .handle_receive = &ps2k_echo_cmd_handle_receive
    }
};



static void ps2k_command_queue_pop(uint8_t device, struct ps2k_device* devc) {
    // dont pop non existing
    if (devc->command_queue_length == 0) return;
    
    // pop first array element
    memmove(devc->command_queue, &(devc->command_queue[1]), sizeof(struct ps2k_command)*(PS2K_COMMAND_QUEUE_MAX_SIZE-1));
    devc->command_queue_length--;
    
    // no longer waiting for response
    devc->waiting_for_response = 0;

    // execute next command if exists
    if (devc->command_queue_length > 0) {
        ps2k_send_command(device, devc);
    }else {
        devc->waiting_for_enable_scanning_ack = 1;
        ps2_enable_scanning(device);
    }
}
static void ps2k_queue_command(uint8_t device, struct ps2k_command cmd) {
    if (device > 1) return;

    struct ps2k_device* devc = &(ps2k_devices[device]);
    
    devc->command_queue[devc->command_queue_length] = cmd;
    devc->command_queue_length++;

    if (devc->command_queue_length == 1) {
        ps2k_send_command(device, devc);
    }
}
static void ps2k_send_command(uint8_t device, struct ps2k_device* devc) {
    if (device > 1) return;
    
    // dont run over current command
    if (devc->waiting_for_response == 1) return;

    // dont send non-existing command
    if (devc->command_queue_length == 0) return;

    // disable scanning if not disabled already
    // dont start disabling if waiting for enabling - when scanning-enable ACK arrives this function will be called again
    if (devc->waiting_for_enable_scanning_ack == 1) {
        return;
    }
    if (devc->scanning_enabled == 1) {
        devc->waiting_for_disable_scanning_ack = 1;
        ps2_disable_scanning(device);
        // stop function execution - when scanning-disable ACK arrives this function will be called again
        return;
    }

    struct ps2k_command* cmd0 = &(devc->command_queue[0]);

    switch (cmd0->cmd) {
        case PS2K_ECHO:
            ps2k_send_echo_cmd(device, devc, cmd0);
            break;
    }
}


static void ps2k_device_send_echo(uint8_t device) {
    ps2k_queue_command(device, (struct ps2k_command){.cmd = PS2K_ECHO});
}
static uint8_t ps2k_device_irq_handler(uint8_t device, uint8_t data) {
    // device is always 0 or 1
    struct ps2k_device* devc = &(ps2k_devices[device]);

    uint8_t irq_handled = 0;

    if (devc->waiting_for_disable_scanning_ack == 1 || devc->waiting_for_enable_scanning_ack == 1) { // check of ack
        if (data == 0xfa) { // ACK
            if (devc->waiting_for_enable_scanning_ack == 1) {
                devc->scanning_enabled = 1;
                devc->waiting_for_enable_scanning_ack = 0;
            }
            if (devc->waiting_for_disable_scanning_ack == 1) {
                devc->scanning_enabled = 0;
                devc->waiting_for_disable_scanning_ack = 0;
            }
            irq_handled = 1;
            // resume sending command execution when scanning enviorment is ready
            if (devc->command_queue_length == 1) {
                ps2k_send_command(device, devc);
            }
        }else if (data == 0xfe) { // resend
            if (devc->waiting_for_disable_scanning_ack == 1) {
                ps2_disable_scanning(device);
            }
            if (devc->waiting_for_enable_scanning_ack == 1) {
                ps2_enable_scanning(device);
            }
            irq_handled = 1;
        }
    }else if (devc->command_queue_length != 0 && devc->waiting_for_response == 1) { // check for a command waiting for response
        // command waiting for response
        struct ps2k_command* cmd0 = &(devc->command_queue[0]);
        
        switch (cmd0->cmd) {
            case PS2K_ECHO:
                irq_handled = ps2k_echo_cmd_handle_receive(device, devc, data);
                break;
        }
    }

    if (irq_handled == 1) {
        print_string("\nhandled ", TM_FORE_COL_GRAY);
    }else {
        print_string("\nnot handled ", TM_FORE_COL_GRAY);
    }
    // print_char('\n', 0);
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
    ps2k_devices[0].scanning_enabled = 1;
    ps2k_devices[0].command_queue_length = 0;
    ps2k_devices[0].waiting_for_response = 0;
    ps2k_devices[0].waiting_for_enable_scanning_ack = 0;
    ps2k_devices[0].waiting_for_disable_scanning_ack = 0;
    ps2k_devices[1].scanning_enabled = 1;
    ps2k_devices[1].command_queue_length = 0;
    ps2k_devices[1].waiting_for_response = 0;
    ps2k_devices[1].waiting_for_enable_scanning_ack = 0;
    ps2k_devices[1].waiting_for_disable_scanning_ack = 0;
    ps2_install_device_driver(&ps2k_device_driver);
}


static void ps2k_send_echo_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0) {
    devc->waiting_for_response = 1;
    devc->current_response_length = 0;
    ps2_send_byte(device, 0xee);
}
// returns 0 if not relevent (irq handler will handle), otherwise 1 (handled)
static uint8_t ps2k_echo_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data) {
    if (data == 0xfe) {
        // resend
        ps2k_send_echo_cmd(device, devc, &(devc->command_queue[0]));
        return 1;
    }
    if (data == 0xee) {
        ps2_declare_echo_success(device);
        ps2k_command_queue_pop(device, devc);
        return 1;
    }
    // not handled
    return 0;
}
