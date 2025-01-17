#pragma once

#include <stdint.h>

#define TM_FORE_COL_BLACK 0x00
#define TM_FORE_COL_BLUE 0x01
#define TM_FORE_COL_GREEN 0x02
#define TM_FORE_COL_CYAN 0x03
#define TM_FORE_COL_RED 0x04
#define TM_FORE_COL_MAGENTA 0x05
#define TM_FORE_COL_BROWN 0x06
#define TM_FORE_COL_GRAY 0x07

#define TM_FORE_COL_BRIGHT 0x08
#define TM_FORE_COL_WHITE 0x0f

#define TM_BACK_COL_BLACK 0x00
#define TM_BACK_COL_BLUE 0x10
#define TM_BACK_COL_GREEN 0x20
#define TM_BACK_COL_CYAN 0x30
#define TM_BACK_COL_RED 0x40
#define TM_BACK_COL_MAGENTA 0x50
#define TM_BACK_COL_BROWN 0x60
#define TM_BACK_COL_GRAY 0x70

#define TM_BACK_COL_BRIGHT 0x80

void text_mode_init();
void clear_screen();
void print_char(char ch, uint8_t col);
void print_string(char* str, uint8_t col);
void print_hex32(int32_t x, uint8_t col);
void print_hex8(int8_t x, uint8_t col);