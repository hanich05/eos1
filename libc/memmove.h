#pragma once

#include <stdint.h>

__attribute__((sysv_abi))
void* memmove(void* dest, const void* src, int32_t n);