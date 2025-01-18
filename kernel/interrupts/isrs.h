#pragma once

#include <stdint.h>

__attribute__((packed))
struct registers_t {
    // old data segment
    uint32_t ds;
    // pushad
    uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
    // pushed by isr-stub
    uint32_t interrupt, error;
    // pushed by the CPU
    uint32_t eip, cs, eflags, esp, ss;
};

void isrs_intall();