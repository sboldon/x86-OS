#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

enum screen_color16 {
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

// TODO: Determine if this struct is necessary after keyboard driver is implemented. Had the
// realization when trying to output interrupt exception messages that holding a pointer to a
// `screen_position` struct is not always possible in situations where output needs to be written.
struct screen_position {
  uint16_t idx;         // Index of the current cell relative to the start of video memory.
  uint16_t attr;        // The foreground and background color written to each cell.
  int x, y;             // The column and row number of the current cell.
};

// Set the screen position via row and column numbers.
void screen_set_pos(struct screen_position *const pos, uint8_t row, uint8_t col);

// Set the foreground and background color to use when drawing characters.
void screen_set_colors(struct screen_position *const pos, uint8_t fg, uint8_t bg);

// Write a null-terminated string to the screen at the provided location.
void screen_write_at(struct screen_position *const pos, const char *str);

// Prepare screen by clearing its contents and enabling the cursor.
void screen_init(struct screen_position *const pos);

// Write a character to the screen at the cursor's current location.
void screen_put(uint8_t ch);

// Write a null-terminated string to the screen starting at the cursor's current location.
void screen_puts(const char* str);

#endif
