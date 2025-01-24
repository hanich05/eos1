#include "ps2k.h"
#include "../ps2.h"
#include "ps2k_scancodes.h"
#include "keys.h"
#include "../../../libc/memmove.h"
#include "../../../kernel/text_mode.h"


/*
    TODO: when sending a command, sending it's byte may timeout. this should be handled!
*/


#define PS2K_COMMAND_QUEUE_MAX_SIZE 16
#define PS2K_COMMAND_MAX_ARGS 2
#define PS2K_COMMAND_MAX_RESPONSE 2
#define PS2K_SCANCODE_SEQUENCE_MAX_LENGTH 10


enum ps2_command_type;
struct ps2k_command;
struct ps2k_command_handler;

struct ps2k_scancode_lookup_entry;
struct ps2k_scancode_match_entry;

struct ps2k_device;



enum ps2_command_type {
    PS2K_ECHO = 0,
    PS2K_ENABLE_SCANNING,
    PS2K_DISABLE_SCANNING,
    PS2K_GET_SCANCODE_SET,

    PS2K_COMMAND_TYPES_AMOUNT
};
struct ps2k_command {
    enum ps2_command_type cmd;
    uint8_t args[PS2K_COMMAND_MAX_ARGS];
};
struct ps2k_command_handler {
    void (*add_to_queue)(uint8_t device, void* args);
    // can assume that:
    // -    devc==&(ps2k_devices[device])
    // -    devc->waiting_for_response==0
    // -    devc->current_response_length==0
    // -    cmd0==&(devc->command_queue[0])
    // -    devc->waiting_for_resonse==0 (or that resend code received and handler recalled)
    void (*send)(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0);
    // returns 0 if not relevent (irq handler will handle), otherwise 1 (handled)
    uint8_t (*handle_receive)(uint8_t device, struct ps2k_device* devc, uint8_t data);
};

struct ps2k_device {
    uint8_t waiting_for_scancode_set    : 1;
    uint8_t scancode_set;

    uint8_t extended_scancode           : 1;
    uint8_t released_scancode           : 1;
    uint8_t current_scancode_sequence_length;
    uint8_t current_scancode_sequence[PS2K_SCANCODE_SEQUENCE_MAX_LENGTH];

    uint8_t command_queue_length;
    struct ps2k_command command_queue[PS2K_COMMAND_QUEUE_MAX_SIZE];
    
    uint8_t waiting_for_response        : 1;
    uint8_t current_response_length;
    uint8_t response[PS2K_COMMAND_MAX_RESPONSE];
};



//////////////////////////
/*** Key Translations ***/
//////////////////////////

static void ps2k_set2_parse_byte(struct ps2k_device* devc, uint8_t byte);
static void ps2k_parse_byte(struct ps2k_device* devc, uint8_t byte);

///////////////////////
/*** Device Driver ***/
///////////////////////

static void ps2k_add_echo_cmd_to_queue(uint8_t device, void* args);
static void ps2k_send_echo_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0);
static uint8_t ps2k_echo_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data);

///////////////////////
/*** Command Queue ***/
///////////////////////

// should be called when the current command has resolved all responses
static void ps2k_command_queue_pop(uint8_t device, struct ps2k_device* devc);
static void ps2k_queue_command(uint8_t device, struct ps2k_command cmd);
// sends the first command in the device's queue
static void ps2k_send_command(uint8_t device, struct ps2k_device* devc);

//////////////////
/*** Commands ***/
//////////////////

static void ps2k_add_enable_scanning_cmd_to_queue(uint8_t device, void* args);
static void ps2k_send_enable_scanning_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0);
static uint8_t ps2k_enable_scanning_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data);

static void ps2k_add_disable_scanning_cmd_to_queue(uint8_t device, void* args);
static void ps2k_send_disable_scanning_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0);
static uint8_t ps2k_disable_scanning_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data);

static void ps2k_add_get_scancode_set_cmd_to_queue(uint8_t device, void* args);
static void ps2k_send_get_scancode_set_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0);
static uint8_t ps2k_get_scancode_set_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data);




// there are only 2 ps2 devices possible (0 or 1)
static struct ps2k_device ps2k_devices[2];

static struct ps2k_command_handler ps2k_command_handlers[PS2K_COMMAND_TYPES_AMOUNT] = {
    [PS2K_ECHO] = (struct ps2k_command_handler) {
        .add_to_queue = &ps2k_add_echo_cmd_to_queue,
        .send = &ps2k_send_echo_cmd,
        .handle_receive = &ps2k_echo_cmd_handle_receive
    },
    [PS2K_ENABLE_SCANNING] = (struct ps2k_command_handler) {
        .add_to_queue = &ps2k_add_enable_scanning_cmd_to_queue,
        .send = &ps2k_send_enable_scanning_cmd,
        .handle_receive = &ps2k_enable_scanning_cmd_handle_receive
    },
    [PS2K_DISABLE_SCANNING] = (struct ps2k_command_handler) {
        .add_to_queue = &ps2k_add_disable_scanning_cmd_to_queue,
        .send = &ps2k_send_disable_scanning_cmd,
        .handle_receive = &ps2k_disable_scanning_cmd_handle_receive
    },
    [PS2K_GET_SCANCODE_SET] = (struct ps2k_command_handler) {
        .add_to_queue = &ps2k_add_get_scancode_set_cmd_to_queue,
        .send = &ps2k_send_get_scancode_set_cmd,
        .handle_receive = &ps2k_get_scancode_set_cmd_handle_receive
    }
};


/////////////////////////////////////////////////////////////////////////////////////
/*   _  __             _____                      _        _    _                  */
/*  | |/ / ___  _  _  |_   _|_ _  __ _  _ _   ___| | __ _ | |_ (_) ___  _ _   ___  */
/*  | ' < / -_)| || |   | | | '_|/ _` || ' \ (_-<| |/ _` ||  _|| |/ _ \| ' \ (_-<  */
/*  |_|\_\\___| \_, |   |_| |_|  \__,_||_||_|/__/|_|\__,_| \__||_|\___/|_||_|/__/  */
/*              |__/                                                               */
/////////////////////////////////////////////////////////////////////////////////////

static void ps2k_set2_parse_byte(struct ps2k_device* devc, uint8_t byte) {
    if (devc->current_scancode_sequence_length >= PS2K_SCANCODE_SEQUENCE_MAX_LENGTH) {
        devc->current_scancode_sequence_length = 0;
        devc->extended_scancode = 0;
        devc->released_scancode = 0;
    }
    
    devc->current_scancode_sequence[devc->current_scancode_sequence_length] = byte;
    devc->current_scancode_sequence_length += 1;

    if (byte == 0xe0) {
        devc->extended_scancode = 1;
        return;
    }
    if (byte == 0xf0) {
        devc->released_scancode = 1;
        return;
    }

    if (devc->extended_scancode == 1) {
        if (byte < PS2K_SCANCODE_SET2_EXTENDED_LOOKUP_TABLE_LENGTH) {
            struct ps2k_scancode_lookup_entry entry = ps2k_scancode_set2_extended_lookup_table[byte];
            if (entry.exists) {
                declare_keystroke(
                    (struct keystroke) {
                        .keycode = entry.keycode,
                        .pressed = (1-devc->released_scancode)
                    }
                );

                devc->current_scancode_sequence_length = 0;
                devc->released_scancode = 0;
                devc->extended_scancode = 0;
                return;
            }
        }
    }else {
        if (byte < PS2K_SCANCODE_SET2_LOOKUP_TABLE_LENGTH) {
            struct ps2k_scancode_lookup_entry entry = ps2k_scancode_set2_lookup_table[byte];
            if (entry.exists) {
                declare_keystroke(
                    (struct keystroke) {
                        .keycode = entry.keycode,
                        .pressed = (1-devc->released_scancode)
                    }
                );

                devc->current_scancode_sequence_length = 0;
                devc->released_scancode = 0;
                return;
            }
        }
    }

    struct ps2k_scancode_match_entry entry;
    for (uint8_t i = 0; i < PS2K_SCANCODE_SET2_MATCHING_LIST_LENGTH; i++) {
        entry = ps2k_scancode_set2_matching_list[i];
        
        if (entry.sequence_length != devc->current_scancode_sequence_length) continue;
        
        uint8_t matching = 1;
        for (uint8_t j = 0; j < entry.sequence_length; j++) {
            if (devc->current_scancode_sequence[j] != entry.sequence[j]) {
                matching = 0;
                break;
            }
        }
        if (matching == 0) continue;

        if (entry.pressed == 1){
            declare_keystroke(
                (struct keystroke) {
                    .keycode = entry.keycode,
                    .pressed = 1
                }
            );
        }
        if (entry.released == 1){
            declare_keystroke(
                (struct keystroke) {
                    .keycode = entry.keycode,
                    .pressed = 0
                }
            );
        }

        devc->current_scancode_sequence_length = 0;
        devc->released_scancode = 0;
        devc->extended_scancode = 0;
        return;
    }
}

static void ps2k_parse_byte(struct ps2k_device* devc, uint8_t byte) {
    switch (devc->scancode_set) {
        case 1: // set 2
            ps2k_set2_parse_byte(devc, byte);
            return;
    }
}


//////////////////////////////////////////////////////////////////
/*   ___             _            ___        _                  */
/*  |   \  ___ __ __(_) __  ___  |   \  _ _ (_)__ __ ___  _ _   */
/*  | |) |/ -_)\ V /| |/ _|/ -_) | |) || '_|| |\ V // -_)| '_|  */
/*  |___/ \___| \_/ |_|\__|\___| |___/ |_|  |_| \_/ \___||_|    */
/*                                                              */
//////////////////////////////////////////////////////////////////

static void ps2k_driver_send_echo(uint8_t device) {
    ps2k_command_handlers[PS2K_ECHO].add_to_queue(device, 0x00);
}
static uint8_t ps2k_driver_irq_handler(uint8_t device, uint8_t data) {
    // device is always 0 or 1
    struct ps2k_device* devc = &(ps2k_devices[device]);

    print_char('\n', 0);
    print_hex8(data, TM_FORE_COL_WHITE);

    uint8_t irq_handled = 0;
    if (devc->command_queue_length != 0 && devc->waiting_for_response == 1) { // check for a command waiting for response
        // command waiting for response
        struct ps2k_command* cmd0 = &(devc->command_queue[0]);
        
        irq_handled = ps2k_command_handlers[cmd0->cmd].handle_receive(device, devc, data);
    }
    if (irq_handled == 1) return 0;
    // should be handled by this function

    ps2k_parse_byte(devc, data);

    return 0;
}
static uint8_t ps2k_driver_on_connect(uint8_t device) {
    struct ps2k_device* devc = &(ps2k_devices[device]);
    
    devc->command_queue_length = 0;
    devc->waiting_for_response = 0;
    
    devc->waiting_for_scancode_set = 1;
    ps2k_command_handlers[PS2K_GET_SCANCODE_SET].add_to_queue(device, 0x00);

    return 0;
}
static struct ps2_device_driver ps2k_device_driver = (struct ps2_device_driver) {
    .long_device_id = 0,
    .device_id      = {0xab, 0x00},
    .send_echo      = &ps2k_driver_send_echo,
    .irq_handler    = &ps2k_driver_irq_handler,
    .on_connect     = &ps2k_driver_on_connect
};
void ps2_keyboard_device_driver_install() {
    ps2_install_device_driver(&ps2k_device_driver);
}


/////////////////////////////////////////////////////////////////////////////////
/*    ___                                    _    ___                          */
/*   / __| ___  _ __   _ __   __ _  _ _   __| |  / _ \  _  _  ___  _  _  ___   */
/*  | (__ / _ \| '  \ | '  \ / _` || ' \ / _` | | (_) || || |/ -_)| || |/ -_)  */
/*   \___|\___/|_|_|_||_|_|_|\__,_||_||_|\__,_|  \__\_\ \_,_|\___| \_,_|\___|  */
/*                                                                             */
/////////////////////////////////////////////////////////////////////////////////

static void ps2k_command_queue_pop(uint8_t device, struct ps2k_device* devc) {
    // dont pop non existing
    if (devc->command_queue_length == 0) return;
    
    // pop first array element
    memmove(devc->command_queue, &(devc->command_queue[1]), sizeof(struct ps2k_command)*(PS2K_COMMAND_QUEUE_MAX_SIZE-1));
    devc->command_queue_length--;
    
    // no longer waiting for response
    devc->waiting_for_response = 0;
    devc->current_response_length = 0;

    // execute next command if exists
    if (devc->command_queue_length > 0) {
        ps2k_send_command(device, devc);
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

    struct ps2k_command* cmd0 = &(devc->command_queue[0]);

    devc->waiting_for_response = 0;
    devc->current_response_length = 0;
    ps2k_command_handlers[cmd0->cmd].send(device, devc, cmd0);
}


///////////////////////////////////////////////////////
/*    ___                                    _       */
/*   / __| ___  _ __   _ __   __ _  _ _   __| | ___  */
/*  | (__ / _ \| '  \ | '  \ / _` || ' \ / _` |(_-<  */
/*   \___|\___/|_|_|_||_|_|_|\__,_||_||_|\__,_|/__/  */
/*                                                   */
///////////////////////////////////////////////////////

static void ps2k_add_echo_cmd_to_queue(uint8_t device, void* args) {
    ps2k_queue_command(device, (struct ps2k_command){.cmd = PS2K_ECHO});
}
static void ps2k_send_echo_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0) {
    devc->waiting_for_response = 1;
    ps2_send_byte(device, 0xee);
}
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

static void ps2k_add_enable_scanning_cmd_to_queue(uint8_t device, void* args) {
    ps2k_queue_command(device, (struct ps2k_command){.cmd = PS2K_ENABLE_SCANNING});
}
static void ps2k_send_enable_scanning_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0) {
    devc->waiting_for_response = 1;
    ps2_send_byte(device, 0xf4);
}
static uint8_t ps2k_enable_scanning_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data) {
    if (data == 0xfe) {
        // resend
        ps2k_send_enable_scanning_cmd(device, devc, &(devc->command_queue[0]));
        
        return 1;
    }
    if (data == 0xfa) {
        // ACK
        ps2k_command_queue_pop(device, devc);
        
        return 1;
    }
    // not handled
    return 0;
}

static void ps2k_add_disable_scanning_cmd_to_queue(uint8_t device, void* args){
    ps2k_queue_command(device, (struct ps2k_command){.cmd = PS2K_DISABLE_SCANNING});
}
static void ps2k_send_disable_scanning_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0) {
    devc->waiting_for_response = 1;
    ps2_send_byte(device, 0xf5);
}
static uint8_t ps2k_disable_scanning_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data) {
    if (data == 0xfe) {
        // resend
        ps2k_send_disable_scanning_cmd(device, devc, &(devc->command_queue[0]));
        
        return 1;
    }
    if (data == 0xfa) {
        // ACK
        ps2k_command_queue_pop(device, devc);
        
        return 1;
    }
    // not handled
    return 0;
}

static void ps2k_add_get_scancode_set_cmd_to_queue(uint8_t device, void* args) {
    ps2k_command_handlers[PS2K_DISABLE_SCANNING].add_to_queue(device, 0x00);
    ps2k_queue_command(device, (struct ps2k_command){.cmd = PS2K_GET_SCANCODE_SET});
    ps2k_command_handlers[PS2K_ENABLE_SCANNING].add_to_queue(device, 0x00);
}
static void ps2k_send_get_scancode_set_cmd(uint8_t device, struct ps2k_device* devc, struct ps2k_command* cmd0) {
    devc->waiting_for_response = 1;
    ps2_send_byte(device, 0xf0);
    ps2_send_byte(device, 0x00);
}
static uint8_t ps2k_get_scancode_set_cmd_handle_receive(uint8_t device, struct ps2k_device* devc, uint8_t data) {
    if (devc->current_response_length == 1) {
        devc->scancode_set = data-1;
        devc->waiting_for_scancode_set = 0;
        ps2k_command_queue_pop(device, devc);
        
        return 1;
    }
    // current_response_length == 0
    
    if (data == 0xfe) {
        // resend
        ps2k_send_get_scancode_set_cmd(device, devc, &(devc->command_queue[0]));
        
        return 1;
    }
    if (data == 0xfa) {
        // ACK
        devc->current_response_length++;
        // not setting devc->response beacuse it wont be used

        return 1;
    }

    // not handled
    return 0;
}
