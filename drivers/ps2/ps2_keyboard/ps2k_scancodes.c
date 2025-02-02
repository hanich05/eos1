#include "ps2k_scancodes.h"
#include "keys.h"


const struct ps2k_scancode_lookup_entry ps2k_scancode_set2_lookup_table[PS2K_SCANCODE_SET2_LOOKUP_TABLE_LENGTH] = {
    [0x00] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x01] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F9},
    [0x02] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x03] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F5},
    [0x04] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F3},
    [0x05] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F1},
    [0x06] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F2},
    [0x07] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F12},
    [0x08] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x09] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F10},
    [0x0a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F8},
    [0x0b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F6},
    [0x0c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F4},
    [0x0d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_TAB},
    [0x0e] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_BACKTICK},
    [0x0f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x10] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x11] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_LEFT_ALT},
    [0x12] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_LEFT_SHIFT},
    [0x13] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x14] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_LEFT_CTRL},
    [0x15] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_Q},
    [0x16] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_1},
    [0x17] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x18] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x19] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x1a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_Z},
    [0x1b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_S},
    [0x1c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_A},
    [0x1d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_W},
    [0x1e] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_2},
    [0x1f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x20] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x21] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_C},
    [0x22] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_X},
    [0x23] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_D},
    [0x24] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_E},
    [0x25] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_4},
    [0x26] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_3},
    [0x27] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x28] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x29] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_SPACE},
    [0x2a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_V},
    [0x2b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F},
    [0x2c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_T},
    [0x2d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_R},
    [0x2e] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_5},
    [0x2f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x30] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x31] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_N},
    [0x32] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_B},
    [0x33] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_H},
    [0x34] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_G},
    [0x35] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_Y},
    [0x36] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_6},
    [0x38] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x37] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x39] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x3a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_M},
    [0x3b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_J},
    [0x3c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_U},
    [0x3d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_7},
    [0x3e] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_8},
    [0x3f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x40] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x41] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_DOT},
    [0x42] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_K},
    [0x43] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_I},
    [0x44] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_O},
    [0x45] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_0},
    [0x46] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_9},
    [0x47] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x48] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x49] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_DOT},
    [0x4a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_SLASH},
    [0x4b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_L},
    [0x4c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_SEMICOLON},
    [0x4d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_P},
    [0x4e] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_MINUS},
    [0x4f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x50] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x51] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x52] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_TICK},
    [0x53] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x54] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_LEFT_SQUARE_BRACKET},
    [0x55] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_EQUAL},
    [0x56] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x57] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x58] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_CAPS_LOCK},
    [0x59] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_RIGHT_SHIFT},
    [0x5a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_ENTER},
    [0x5b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_RIGHT_SQUARE_BRACKET},
    [0x5c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x5d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_BACKSLASH},
    [0x5e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x5f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x60] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x61] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x62] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x63] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x64] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x65] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x66] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_BACKSPACE},
    [0x67] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x68] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x69] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_1},
    [0x6a] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x6b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_4},
    [0x6c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_7},
    [0x6d] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x6e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x6f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x70] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_0},
    [0x71] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_DOT},
    [0x72] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_2},
    [0x73] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_5},
    [0x74] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_6},
    [0x75] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_8},
    [0x76] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_ESCAPE},
    [0x77] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_NUM_LOCK},
    [0x78] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F11},
    [0x79] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_PLUS},
    [0x7a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_3},
    [0x7b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_MINUS},
    [0x7c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_ASTERISK},
    [0x7d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_9},
    [0x7e] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_SCROLL_LOCK},
    [0x7f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x80] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x81] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x82] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x83] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_F7},
};
const struct ps2k_scancode_lookup_entry ps2k_scancode_set2_extended_lookup_table[PS2K_SCANCODE_SET2_EXTENDED_LOOKUP_TABLE_LENGTH] = {
    [0x00] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x01] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x02] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x03] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x04] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x05] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x06] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x07] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x08] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x09] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x0a] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x0b] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x0c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x0d] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x0e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x0f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x10] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WWW_SEARCH},
    [0x11] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_RIGHT_ALT},
    [0x12] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x13] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x14] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_RIGHT_CTRL},
    [0x15] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_PREVIOUS_TRACK},
    [0x16] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x17] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x18] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WWW_FAVOURITES},
    [0x19] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x1a] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x1b] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x1c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x1d] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x1e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x1f] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_LEFT_GUI},
    [0x20] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WWW_REFRESH},
    [0x21] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_VOLUME_DOWN},
    [0x22] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x23] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_MUTE},
    [0x24] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x25] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x26] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x27] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_RIGHT_GUI},
    [0x28] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WWW_STOP},
    [0x29] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x2a] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x2b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_CALCULATOR},
    [0x2c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x2d] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x2e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x2f] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_APPS},
    [0x30] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WWW_FORWARD},
    [0x31] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x32] = (struct ps2k_scancode_lookup_entry){.exists = 0, .keycode = KC_VOLUME_UP},
    [0x33] = (struct ps2k_scancode_lookup_entry){.exists = 1},
    [0x34] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_PLAY_PAUSE},
    [0x35] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x36] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x37] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_POWER},
    [0x38] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WWW_BACK},
    [0x39] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x3a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WWW_HOME},
    [0x3b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_STOP},
    [0x3c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x3d] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x3e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x3f] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_SLEEP},
    [0x40] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_MY_COMPUTER},
    [0x41] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x42] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x43] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x44] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x45] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x46] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x47] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x48] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_EMAIL},
    [0x49] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x4a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_MEDIA_SELECT},
    [0x4b] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x4c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x4d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_NEXT_TRACK},
    [0x4e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x4f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x50] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_MEDIA_SELECT},
    [0x51] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x52] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x53] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x54] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x55] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x56] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x57] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x58] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x59] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x5a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_KEYPAD_ENTER},
    [0x5b] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x5c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x5d] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x5e] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_WAKE},
    [0x5f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x60] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x61] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x61] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x62] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x63] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x64] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x65] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x66] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x67] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x68] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x69] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_END},
    [0x6a] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x6b] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_CURSOR_LEFT},
    [0x6c] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_HOME},
    [0x6d] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x6e] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x6f] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x70] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_INSERT},
    [0x71] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_DELETE},
    [0x72] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_CURSOR_DOWN},
    [0x73] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x74] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_CURSOR_RIGHT},
    [0x75] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_CURSOR_UP},
    [0x76] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x77] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x78] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x79] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x7a] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_PAGE_DOWN},
    [0x7b] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x7c] = (struct ps2k_scancode_lookup_entry){.exists = 0},
    [0x7d] = (struct ps2k_scancode_lookup_entry){.exists = 1, .keycode = KC_PAGE_UP},
};
static const uint8_t kc_print_pressed_sequence[] = {0xe0, 0x12, 0xe0, 0x7c};
static const uint8_t kc_print_released_sequence[] = {0xe0, 0xf0, 0x7c, 0xe0, 0xf0, 0x12};
static const uint8_t kc_pause_sequence[] = {0xe1, 0x14, 0x77, 0xe1, 0xf0, 0x14, 0xf0, 0x77};
const struct ps2k_scancode_match_entry ps2k_scancode_set2_matching_list[PS2K_SCANCODE_SET2_MATCHING_LIST_LENGTH] = {
    (struct ps2k_scancode_match_entry){
        .sequence = kc_print_pressed_sequence,
        .sequence_length = 4,
        .keycode = KC_PRINT_SCREEN,
        .pressed = 1,
        .released = 0
    },
    (struct ps2k_scancode_match_entry){
        .sequence = kc_print_released_sequence,
        .sequence_length = 6,
        .keycode = KC_PRINT_SCREEN,
        .pressed = 0,
        .released = 1
    },
    (struct ps2k_scancode_match_entry){
        .sequence = kc_pause_sequence,
        .sequence_length = 8,
        .keycode = KC_PAUSE,
        .pressed = 1,
        .released = 1
    }
};