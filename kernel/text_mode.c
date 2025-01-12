#include "text_mode.h"
#include "../libc/memmove.h"
#include "../libc/memset.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef struct __attribute__((packed)) {
    char ch;
    int8_t col;
} tm_char_t;

#define TM_SCREEN ((tm_char_t*)0xb8000)

static struct {
    uint8_t x;
    uint8_t y;
} curs_pos;

static void move_lines_up() {
    memmove(TM_SCREEN, &(TM_SCREEN[SCREEN_WIDTH]), 2*SCREEN_WIDTH*(SCREEN_HEIGHT-1));
    memset(&(TM_SCREEN[SCREEN_WIDTH*(SCREEN_HEIGHT-1)]), 0x00, 2*SCREEN_WIDTH);
}

static void wrap_curs() {
    if (curs_pos.x >= SCREEN_WIDTH) {
        curs_pos.x -= SCREEN_WIDTH;
    }
    while (curs_pos.y >= SCREEN_HEIGHT) {
        move_lines_up();
        curs_pos.y--;
    }
}

void text_mode_init() {
    curs_pos.x = 0;
    curs_pos.y = 0;
}

void clear_screen() {
    memset(TM_SCREEN, 0x00, 2*SCREEN_WIDTH*SCREEN_HEIGHT);
}

void print_char(char ch, int8_t col) {
    wrap_curs();
    switch (ch) {
        case '\n':
            curs_pos.x = 0;
            curs_pos.y++;
            wrap_curs();
            break;
        
        case '\r':
            curs_pos.x = 0;
            break;

        default:
            TM_SCREEN[curs_pos.y*SCREEN_WIDTH + curs_pos.x] = (tm_char_t){.ch=ch,.col=col};
            curs_pos.x++;
            break;
    }
}

void print_string(char* str, int8_t col) {
    int32_t i = 0;
    while (str[i] != '\0') {
        print_char(str[i], col);
        i++;
    }
}
