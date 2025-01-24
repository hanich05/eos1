#include "keys.h"

struct keycode_data keycodes_data[KEYCODES_AMOUNT] = {
    [KC_F1] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F2] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F3] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F4] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F5] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F6] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F7] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F8] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F9] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F10] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F11] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_F12] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    
    [KC_SPACE] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = ' '
    },
    [KC_BACKSPACE] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_ESCAPE] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_ENTER] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '\n'
    },
    [KC_TAB] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0
    },

    [KC_END] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_HOME] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_PAGE_DOWN] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_PAGE_UP] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_CURSOR_LEFT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_CURSOR_DOWN] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_CURSOR_RIGHT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_CURSOR_UP] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_INSERT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_DELETE] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },

    [KC_LEFT_ALT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_RIGHT_ALT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_LEFT_SHIFT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_RIGHT_SHIFT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_LEFT_CTRL] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_RIGHT_CTRL] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    
    [KC_TICK] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '\''
    },
    [KC_BACKTICK] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '`'
    },
    [KC_COMMA] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = ','
    },
    [KC_SEMICOLON] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = ';'
    },
    
    [KC_LEFT_BRACKET] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '('
    },
    [KC_RIGHT_BRACKET] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = ')'
    },
    [KC_LEFT_SQUARE_BRACKET] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '['
    },
    [KC_RIGHT_SQUARE_BRACKET] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = ']'
    },
    [KC_LEFT_SQUIGLE_BRACKET] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '{'
    },
    [KC_RIGHT_SQUIGLE_BRACKET] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '}'
    },
    
    [KC_BACKSLASH] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '\\'
    },
    [KC_SLASH] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '/'
    },

    [KC_DOT] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '.'
    },
    [KC_EQUAL] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '='
    },
    [KC_MINUS] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '-'
    },
    [KC_PLUS] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '+'
    },
    [KC_ASTERISK] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '*'
    },
    
    [KC_0] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '0'
    },
    [KC_1] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '1'
    },
    [KC_2] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '2'
    },
    [KC_3] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '3'
    },
    [KC_4] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '4'
    },
    [KC_5] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '5'
    },
    [KC_6] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '6'
    },
    [KC_7] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '7'
    },
    [KC_8] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '8'
    },
    [KC_9] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '9'
    },
    
    [KC_CAPS_LOCK] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_SCROLL_LOCK] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_NUM_LOCK] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },

    [KC_KEYPAD_DOT] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '.'
    },
    [KC_KEYPAD_EQUAL] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '='
    },
    [KC_KEYPAD_MINUS] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '-'
    },
    [KC_KEYPAD_PLUS] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '+'
    },
    [KC_KEYPAD_ASTERISK] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '*'
    },
    [KC_KEYPAD_ENTER] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    
    [KC_KEYPAD_0] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '0'
    },
    [KC_KEYPAD_1] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '1'
    },
    [KC_KEYPAD_2] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '2'
    },
    [KC_KEYPAD_3] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '3'
    },
    [KC_KEYPAD_4] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '4'
    },
    [KC_KEYPAD_5] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '5'
    },
    [KC_KEYPAD_6] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '6'
    },
    [KC_KEYPAD_7] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '7'
    },
    [KC_KEYPAD_8] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '8'
    },
    [KC_KEYPAD_9] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = '9'
    },

    [KC_A] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'a'
    },
    [KC_B] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'b'
    },
    [KC_C] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'c'
    },
    [KC_D] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'd'
    },
    [KC_E] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'e'
    },
    [KC_F] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'f'
    },
    [KC_G] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'g'
    },
    [KC_H] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'h'
    },
    [KC_I] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'i'
    },
    [KC_J] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'j'
    },
    [KC_K] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'k'
    },
    [KC_L] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'l'
    },
    [KC_M] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'm'
    },
    [KC_N] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'n'
    },
    [KC_O] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'o'
    },
    [KC_P] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'p'
    },
    [KC_Q] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'q'
    },
    [KC_R] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'r'
    },
    [KC_S] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 's'
    },
    [KC_T] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 't'
    },
    [KC_U] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'u'
    },
    [KC_V] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'v'
    },
    [KC_W] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'w'
    },
    [KC_X] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'x'
    },
    [KC_Y] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'y'
    },
    [KC_Z] = (struct keycode_data){
        .has_ascii = 1,
        .ascii = 'z'
    },

    [KC_WWW_SEARCH] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_WWW_FAVOURITES] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_WWW_REFRESH] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_WWW_STOP] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_WWW_FORWARD] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_WWW_BACK] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_WWW_HOME] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },

    [KC_PREVIOUS_TRACK] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_NEXT_TRACK] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_VOLUME_DOWN] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_VOLUME_UP] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_PLAY_PAUSE] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_STOP] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_MUTE] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_MEDIA_SELECT] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },

    [KC_RIGHT_GUI] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_LEFT_GUI] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_CALCULATOR] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_MY_COMPUTER] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_EMAIL] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_APPS] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },

    [KC_POWER] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_SLEEP] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_WAKE] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },

    [KC_PRINT_SCREEN] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
    [KC_PAUSE] = (struct keycode_data){
        .has_ascii = 0,
        .ascii = 0x00
    },
};


#include "../../../kernel/text_mode.h"

void declare_keystroke(struct keystroke ks) {
    struct keycode_data kcd = keycodes_data[ks.keycode];
    
    print_string("\nkeycode ", TM_FORE_COL_GRAY);
    print_hex8(ks.keycode, TM_FORE_COL_WHITE);
    
    if (kcd.has_ascii == 1) {
        print_string(" ('", TM_FORE_COL_GRAY);
        switch (kcd.ascii) {
            case '\n':
                print_string("\\n", TM_FORE_COL_WHITE);
                break;
            default:
                print_char(kcd.ascii, TM_FORE_COL_WHITE);
                break;
        }
        print_string("') ", TM_FORE_COL_GRAY);
    }else {
        print_string("       ", TM_FORE_COL_GRAY);
    }

    if (ks.pressed == 1) {
        print_string("pressed", TM_FORE_COL_GREEN);
    }else {
        print_string("released", TM_FORE_COL_BROWN);
    }
}
