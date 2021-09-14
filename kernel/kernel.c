#include <stddef.h>
#include "vga_screen.h"

#if defined(__linux__)
	#error "kernel must be built with a cross-compiler"
#elif !defined(__i386__)
	#error "kernel must be built with an x86-elf compiler"
#endif

// Example monolithic kernel.
void main(void) {
  // TODO: Move struct creation inside keyboard driver once it is created. The struct will be of an
  // abstract type for all client code, but cannot actualize this until dynamic allocation is
  // implemented.
  struct screen_position pos;
  set_screen_pos(&pos, 0, 0);
  set_colors(&pos, LIGHT_MAGENTA, DARK_GRAY);
  init_screen(&pos);
  print(&pos, "Hello\nWorld!");
}

