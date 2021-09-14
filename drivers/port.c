#include "./include/port.h"

// Load EDX with port and set byte to value of AL after reading.
uint8_t inb(uint16_t port) {
  uint8_t byte;
  __asm__("in %%dx, %%al" : "=a" (byte) : "d" (port));
  return byte;
}


// Load EDX with port and load EAX with byte.
void outb(uint16_t port, uint8_t byte) {
  __asm__("out %%al, %%dx" : :"a" (byte), "d" (port));
}


// Load EDX with port and set word to value of AX after reading.
uint16_t inw(uint16_t port) {
  uint16_t word;
  __asm__("in %%dx, %%ax" : "=a" (word) : "d" (port));
  return word;
}


// Load EDX with port and load EAX with word.
void outw(uint16_t port, uint16_t word) {
  __asm__("out %%ax, %%dx" : :"a" (word), "d" (port));
}

