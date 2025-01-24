#pragma once

#include <stdint.h>

enum keycode {
    KC_F1 = 0,
    KC_F2,
    KC_F3,
    KC_F4,
    KC_F5,
    KC_F6,
    KC_F7,
    KC_F8,
    KC_F9,
    KC_F10,
    KC_F11,
    KC_F12,
    
    KC_SPACE,
    KC_BACKSPACE,
    KC_ESCAPE,
    KC_ENTER,
    KC_TAB,

    KC_END,
    KC_HOME,
    KC_PAGE_DOWN,
    KC_PAGE_UP,
    KC_CURSOR_LEFT,
    KC_CURSOR_DOWN,
    KC_CURSOR_RIGHT,
    KC_CURSOR_UP,
    KC_INSERT,
    KC_DELETE,

    KC_LEFT_ALT,
    KC_RIGHT_ALT,
    KC_LEFT_SHIFT,
    KC_RIGHT_SHIFT,
    KC_LEFT_CTRL,
    KC_RIGHT_CTRL,
    
    KC_TICK,
    KC_BACKTICK,
    KC_COMMA,
    KC_SEMICOLON,
    
    KC_LEFT_BRACKET,
    KC_RIGHT_BRACKET,
    KC_LEFT_SQUARE_BRACKET,
    KC_RIGHT_SQUARE_BRACKET,
    KC_LEFT_SQUIGLE_BRACKET,
    KC_RIGHT_SQUIGLE_BRACKET,
    
    KC_BACKSLASH,
    KC_SLASH,

    KC_DOT,
    KC_EQUAL,
    KC_MINUS,
    KC_PLUS,
    KC_ASTERISK,
    
    KC_0,
    KC_1,
    KC_2,
    KC_3,
    KC_4,
    KC_5,
    KC_6,
    KC_7,
    KC_8,
    KC_9,
    
    KC_CAPS_LOCK,
    KC_SCROLL_LOCK,
    KC_NUM_LOCK,

    KC_KEYPAD_DOT,
    KC_KEYPAD_EQUAL,
    KC_KEYPAD_MINUS,
    KC_KEYPAD_PLUS,
    KC_KEYPAD_ASTERISK,
    KC_KEYPAD_ENTER,
    
    KC_KEYPAD_0,
    KC_KEYPAD_1,
    KC_KEYPAD_2,
    KC_KEYPAD_3,
    KC_KEYPAD_4,
    KC_KEYPAD_5,
    KC_KEYPAD_6,
    KC_KEYPAD_7,
    KC_KEYPAD_8,
    KC_KEYPAD_9,

    KC_A,
    KC_B,
    KC_C,
    KC_D,
    KC_E,
    KC_F,
    KC_G,
    KC_H,
    KC_I,
    KC_J,
    KC_K,
    KC_L,
    KC_M,
    KC_N,
    KC_O,
    KC_P,
    KC_Q,
    KC_R,
    KC_S,
    KC_T,
    KC_U,
    KC_V,
    KC_W,
    KC_X,
    KC_Y,
    KC_Z,

    KC_WWW_SEARCH,
    KC_WWW_FAVOURITES,
    KC_WWW_REFRESH,
    KC_WWW_STOP,
    KC_WWW_FORWARD,
    KC_WWW_BACK,
    KC_WWW_HOME,

    KC_PREVIOUS_TRACK,
    KC_NEXT_TRACK,
    KC_VOLUME_DOWN,
    KC_VOLUME_UP,
    KC_PLAY_PAUSE,
    KC_STOP,
    KC_MUTE,
    KC_MEDIA_SELECT,

    KC_RIGHT_GUI,
    KC_LEFT_GUI,
    KC_CALCULATOR,
    KC_MY_COMPUTER,
    KC_EMAIL,
    KC_APPS,

    KC_POWER,
    KC_SLEEP,
    KC_WAKE,

    KC_PRINT_SCREEN,
    KC_PAUSE,


    KEYCODES_AMOUNT
};

struct keycode_data {
    uint8_t has_ascii   : 1;
    char ascii;
};

struct keystroke {
    enum keycode keycode;
    // 0 = released, 1 = pressed
    uint8_t pressed     : 1;
};

extern struct keycode_data keycodes_data[KEYCODES_AMOUNT];

void declare_keystroke(struct keystroke ks);