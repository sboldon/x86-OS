#include "screen.h"
#include "string.h"
#include "port.h"
#include <stdint.h>

// Start of video memory.
static volatile uint16_t *const vmem = (uint16_t*)0xb8000;

#define WIDTH 80
#define HEIGHT 25

// VGA I/O ports
#define SCREEN_ADDR_REG 0x3D4
#define SCREEN_DATA_REG 0x3D5

// Cursor Start Register
// CD = cursor disable
// ┌───┬───┬────┬────┬────┬────┬────┬────┐
// │ 7 │ 6 │ 5  │ 4  │ 3  │ 2  │ 1  │ 0  │
// ├───┼───┼────┼────┴────┴────┴────┴────┤
// │   │   │ CD │ Cursor Scan Line Start │
// └───┴───┴────┴────────────────────────┘
#define CURSOR_START_REG_IDX 0x0A
enum cursor_start_reg_mask {
  CURSOR_SCAN_LINE_START = 0x1Fu,
  CD = 0x20u,
};

// Cursor End Register
// ┌───┬──────┬──────┬────┬────┬────┬───┬───┐
// │ 7 │   6  │  5   │ 4  │ 3  │ 2  │ 1 │ 0 │
// ├───┼──────┴──────┼────┴────┴────┴───┴───┤
// │   │ Cursor Skew │ Cursor Scan Line End │
// └───┴─────────────┴──────────────────────┘
#define CURSOR_END_REG_IDX 0x0B
enum cursor_end_reg_mask {
  CURSOR_SCAN_LINE_END = 0x1fu,
  CURSOR_SKEW = 0x60u,
};

#define CURSOR_LOC_HI_REG_IDX 0x0E // Bits 15-8 of cursor location.
#define CURSOR_LOC_LO_REG_IDX 0x0F // Bits 0-7 of cursor location.

#define VERT_DISPLAY_END_REG_IDX 0x12 // Bits 0-7 of vertical display end.

// Set the masked bits of `val` to `new_val`.
// TODO: This macro will be useful in other files requiring bit manipulation
#define SET_MASKED_BITS(val, mask, new_val) ((val & ~mask) | (new_val & mask))

// Create the color byte for a video memory cell with the specified foreground and background color.
#define SET_COLOR(fg, bg) ((bg << 4u) | fg)

static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
  // Set cursor disable bit to 0 and cursor scan line to cursor start.
  outb(SCREEN_ADDR_REG, CURSOR_START_REG_IDX);
  uint8_t data_reg_val = inb(SCREEN_DATA_REG);
  outb(SCREEN_DATA_REG, SET_MASKED_BITS(data_reg_val, ~CD & CURSOR_SCAN_LINE_START, cursor_start));

  // Set cursor scan line to cursor end
  outb(SCREEN_ADDR_REG, CURSOR_END_REG_IDX);
  data_reg_val = inb(SCREEN_DATA_REG);
  outb(SCREEN_DATA_REG, SET_MASKED_BITS(data_reg_val, CURSOR_SCAN_LINE_END, cursor_end));
}


// Set cursor position to `loc` indices past the start of video memory.
static void update_cursor(uint16_t loc) {
  outb(SCREEN_ADDR_REG, CURSOR_LOC_LO_REG_IDX);
  outb(SCREEN_DATA_REG, loc & 0xFF);
  outb(SCREEN_ADDR_REG, CURSOR_LOC_HI_REG_IDX);
  outb(SCREEN_DATA_REG, (loc >> 8) & 0xFF);
}


// The index of the video memory cell that the cursor is currently at.
static uint16_t cursor_loc(void) {
  uint16_t loc;
  outb(SCREEN_ADDR_REG, CURSOR_LOC_LO_REG_IDX);
  loc = inb(SCREEN_DATA_REG);
  outb(SCREEN_ADDR_REG, CURSOR_LOC_HI_REG_IDX);
  return (inb(SCREEN_DATA_REG) << 8) | loc;
}


inline static void draw_char(struct screen_position *const pos, uint8_t ch) {
  if (pos->x >= WIDTH) {
    pos->x = 0;
    pos->y += 1;
  }
  if (pos->y >= HEIGHT) {
    // TODO: Scroll screen contents up using `memcpy`.
  }
  if (ch >= ' ') {
    // Character is printable.
    vmem[pos->idx++] = pos->attr | ch;
    pos->x += 1;
  }
  else switch(ch) {
    case '\n':
      // Move cursor to the start of the next line.
      pos->idx += WIDTH - pos->x;
      pos->x = 0;
      pos->y += 1;
      break;
    case '\r':
      // Return cursor to the start of the current line.
      pos->idx -= pos->x;
      pos->x = 0;
      break;
    case '\b':
      // Move cursor to the previous character.
      if (pos->x > 0) {
        pos->idx -= 1;
        pos->x -= 1;
      }
      break;
  }
  update_cursor(pos->idx);
}


// Wipe screen contents by setting every character to space. Additionally, set the foreground and
// background color of every cell.
static void clear_screen(uint16_t attr) {
  uint16_t cell = attr | ' ';
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      vmem[i * WIDTH + j] = cell;
    }
  }
}


void screen_set_pos(struct screen_position *const pos, uint8_t row, uint8_t col) {
  pos->x = col >= WIDTH ? WIDTH : col;
  pos->y = row >= HEIGHT ? HEIGHT : row;
  pos->idx = pos->x * WIDTH + pos->y;
}


void screen_set_colors(struct screen_position *const pos, uint8_t fg, uint8_t bg) {
  // The color byte is shifted a byte left so that it can be combined with a character using just a
  // bitwise OR.
  pos->attr = SET_COLOR(fg, bg) << 8u;
}


void screen_write_at(struct screen_position *const pos, const char *str) {
  while (*str) { draw_char(pos, *str++); }
}


void screen_init(struct screen_position *const pos) {
  screen_set_pos(pos, 0, 0);
  clear_screen(pos->attr);
  enable_cursor(0, 15);
  update_cursor(0);
}


void screen_put(uint8_t ch) {
  uint16_t cell_idx = cursor_loc();
  struct screen_position pos = {
    .idx = cell_idx,
    .attr = vmem[cell_idx] & 0xFF00, // Clear character byte.
    .x = cell_idx % WIDTH,
    .y = cell_idx / WIDTH,
  };
  draw_char(&pos, ch);
}


void screen_puts(const char* str) {
  uint16_t cell_idx = cursor_loc();
  struct screen_position pos = {
    .idx = cell_idx,
    .attr = vmem[cell_idx] & 0xFF00, // Clear character byte.
    .x = cell_idx % WIDTH,
    .y = cell_idx / WIDTH,
  };
  screen_write_at(&pos, str);
}
