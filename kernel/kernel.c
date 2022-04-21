#if defined(__linux__)
  #error "kernel must be built with a cross-compiler"
#elif !defined(__i386__)
  #error "kernel must be built with an x86-elf compiler"
#endif

#include "idt.h"
#include "screen.h"

// Example monolithic kernel :)
void main(void) {
  idt_init();

  struct screen_position pos;
  screen_set_colors(&pos, LIGHT_MAGENTA, DARK_GRAY);
  screen_init(&pos);
  screen_puts("Hello World!\n");

  // Test interrupt handling.
  __asm__ volatile ("int $0x0");
  for (;;);
}

