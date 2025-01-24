#include "text_mode.h"
#include "../libc/memmove.h"
#include "../libc/memset.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef struct __attribute__((packed)) {
    char ch;
    uint8_t col;
} tm_char_t;

#define TM_SCREEN ((tm_char_t*)0xb8000)

struct u8_vec2 curs_pos;

static void move_lines_up() {
    memmove(TM_SCREEN, &(TM_SCREEN[SCREEN_WIDTH]), 2*SCREEN_WIDTH*(SCREEN_HEIGHT-1));
    memset(&(TM_SCREEN[SCREEN_WIDTH*(SCREEN_HEIGHT-1)]), 0x00, 2*SCREEN_WIDTH);
}

static void wrap_curs() {
    if (curs_pos.x >= SCREEN_WIDTH) {
        curs_pos.x -= SCREEN_WIDTH;
        curs_pos.y++;
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

void print_char(char ch, uint8_t col) {
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

void print_string(char* str, uint8_t col) {
    int32_t i = 0;
    while (str[i] != '\0') {
        print_char(str[i], col);
        i++;
    }
}

void print_hex32(uint32_t x, uint8_t col) {
    uint32_t a = x;
    for (int32_t i = 0; i < 8; i++) {
        int8_t half_byte_val = ((a >> 28) & 0x0f);
        char c;
        if (half_byte_val < 10) {
            c = '0' + half_byte_val;
        }else {
            c = 'A' + half_byte_val-10;
        }
        print_char(c, col);
        a = a << 4;
    }
}

void print_hex8(uint8_t x, uint8_t col) {
    int8_t a = x;
    for (int32_t i = 0; i < 2; i++) {
        int8_t half_byte_val = ((a >> 4) & 0x0f);
        char c;
        if (half_byte_val < 10) {
            c = '0' + half_byte_val;
        }else {
            c = 'A' + half_byte_val-10;
        }
        print_char(c, col);
        a = a << 4;
    }
}

void print_memory_block(uint32_t from, uint32_t to) {
    uint8_t* memptr = (uint8_t*)(from & 0xfffffff0);

    while ((uint32_t)memptr < to) {
        print_char('\n', 0);
        print_hex32((uint32_t)memptr, TM_FORE_COL_BLUE);
        print_string("    ", 0);
        
        for (uint8_t j = 0; j < 16; j++) {
            print_hex8(*memptr, TM_FORE_COL_WHITE);
            print_char(' ', 0);
            if (j%4 == 3) print_char(' ', 0);
            memptr++;
        }
    }
}

void print_bin8(uint8_t x, uint8_t col) {
    int8_t a = x;
    for (int32_t i = 0; i < 8; i++) {
        int8_t bit = ((a >> 7) & 1);
        print_char('0'+bit, col);
        a = a << 1;
    }
}