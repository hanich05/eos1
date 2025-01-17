#pragma once

#include <stdint.h>

__attribute__((sysv_abi))
void outb(uint16_t port, uint8_t byte);

__attribute__((sysv_abi))
uint8_t inb(uint16_t port);