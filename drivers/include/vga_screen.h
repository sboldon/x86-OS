#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

enum color16 {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  GRAY,
  DARK_GRAY,
  LIGHT_BLUE,
  LIGHT_GREEN,
  LIGHT_CYAN,
  LIGHT_RED,
  LIGHT_MAGENTA,
  LIGHT_BROWN,
  WHITE
};

struct screen_position {
  uint16_t idx;         // Index of the current cell relative to the start of video memory.
  int x, y;             // The column and row number of the current cell.
  uint16_t attr;         // The foreground and background color written to each cell.
};

// Set the screen position via row and column numbers.
void set_screen_pos(struct screen_position *const pos, int row, int col);

// Set the foreground and background color to use when drawing characters.
void set_colors(struct screen_position *const pos, int fg, int bg);

// Draw a null-terminated string to the screen starting at the given position.
void print(struct screen_position *const pos, const char *str) ;

// Prepare screen by clearing its contents and enabling the cursor.
void init_screen(struct screen_position *const pos);

#endif

