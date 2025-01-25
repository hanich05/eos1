#include "paging.h"

#include <stdint.h>

__attribute__((packed))
struct page_directory_entry {
    union {
        struct constant {
            uint32_t preserve24     : 24;
            uint8_t ps              : 1;
            uint8_t preserve1       : 1;
            uint8_t a               : 1;
            uint8_t pcd             : 1;
            uint8_t pwt             : 1;
            uint8_t us              : 1;
            uint8_t rw              : 1;
            uint8_t p               : 1;
        };
        struct page_size_large {
            uint32_t address_31_22  : 10;
            uint8_t reserved        : 1;
            uint8_t address_39_32;
            uint8_t pat             : 1;
            uint8_t avl             : 3;
            uint8_t g               : 1;
            uint8_t ps              : 1;
            uint8_t d               : 1;
            uint8_t a               : 1;
            uint8_t pcd             : 1;
            uint8_t pwt             : 1;
            uint8_t us              : 1;
            uint8_t rw              : 1;
            uint8_t p               : 1;
        };
        struct page_size_small {
            uint32_t address_31_12  : 20;
            uint8_t avl4            : 4;
            uint8_t ps              : 1;
            uint8_t avl1            : 1;
            uint8_t a               : 1;
            uint8_t pcd             : 1;
            uint8_t pwt             : 1;
            uint8_t us              : 1;
            uint8_t rw              : 1;
            uint8_t p               : 1;
        };
    };
};
