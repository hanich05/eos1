#pragma once

#include <stdint.h>


// last non-extended scancode in set 2 is 0x83
#define PS2K_SCANCODE_SET2_LOOKUP_TABLE_LENGTH 0x84
// last extended second byte in set 2 is 0x7d
#define PS2K_SCANCODE_SET2_EXTENDED_LOOKUP_TABLE_LENGTH 0x7e
#define PS2K_SCANCODE_SET2_MATCHING_LIST_LENGTH 3


struct ps2k_scancode_lookup_entry {
    uint8_t exists      : 1;
    uint8_t keycode;
};
struct ps2k_scancode_match_entry {
    const uint8_t* sequence;
    uint8_t sequence_length;
    uint8_t keycode;
    uint8_t pressed     : 1;
    uint8_t released    : 1;
};


extern const struct ps2k_scancode_lookup_entry ps2k_scancode_set2_lookup_table[PS2K_SCANCODE_SET2_LOOKUP_TABLE_LENGTH];
extern const struct ps2k_scancode_lookup_entry ps2k_scancode_set2_extended_lookup_table[PS2K_SCANCODE_SET2_EXTENDED_LOOKUP_TABLE_LENGTH];
extern const struct ps2k_scancode_match_entry  ps2k_scancode_set2_matching_list[PS2K_SCANCODE_SET2_MATCHING_LIST_LENGTH];

