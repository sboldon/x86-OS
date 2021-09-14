// Low-level port I/O functionality for read/writing bytes and words.
#ifndef PORT_H
#define PORT_H
#include <stdint.h>

uint8_t inb(uint16_t port);

void outb(uint16_t port, uint8_t byte);

uint16_t inw(uint16_t port);

void outw(uint16_t port, uint16_t word);

#endif

