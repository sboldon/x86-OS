#include "vga_screen.h"
#include "port.h"

// Start of video memory.
static volatile uint16_t *const vmem = (uint16_t*)0xb8000;

#define WIDTH 80
#define HEIGHT 25

/** VGA I/O ports **/
#define SCREEN_ADDR_REG 0x3d4
#define SCREEN_DATA_REG 0x3d5

// Cursor Start Register
// CD = cursor disable
// ┌───┬───┬────┬────┬────┬────┬────┬────┐
// │ 7 │ 6 │ 5  │ 4  │ 3  │ 2  │ 1  │ 0  │
// ├───┼───┼────┼────┴────┴────┴────┴────┤
// │   │   │ CD │ Cursor Scan Line Start │
// └───┴───┴────┴────────────────────────┘
#define CURSOR_START_REG_IDX 0x0A
enum cursor_start_reg_mask {
  CURSOR_SCAN_LINE_START = 0x1fU,
  CD = 0x20U,
};

// Cursor End Register
// ┌───┬──────┬──────┬────┬────┬────┬───┬───┐
// │ 7 │ 6    │ 5    │ 4  │ 3  │ 2  │ 1 │ 0 │
// ├───┼──────┴──────┼────┴────┴────┴───┴───┤
// │   │ Cursor Skew │ Cursor Scan Line End │
// └───┴─────────────┴──────────────────────┘
#define CURSOR_END_REG_IDX 0x0B
enum cursor_end_reg_mask {
  CURSOR_SCAN_LINE_END = 0x1fU,
  CURSOR_SKEW = 0x60U,
};

#define CURSOR_LOC_HI_REG_IDX 0x0E // Bits 15-8 of cursor location.
#define CURSOR_LOC_LO_REG_IDX 0x0F // Bits 0-7 of cursor location.

#define VERT_DISPLAY_END_REG_IDX 0x12 // Bits 0-7 of vertical display end.

// Set the masked bits of val to new_val.
// TODO: This macro will be useful in other files requiring bit manipulation
#define SET_MASKED_BITS(val, mask, new_val) ((val & ~mask) | (new_val & mask))


static void enable_cursor(int cursor_start, int cursor_end) {
  // Set cursor disable bit to 0 and cursor scan line to cursor start.
  outb(SCREEN_ADDR_REG, CURSOR_START_REG_IDX);
  uint8_t data_reg_val = inb(SCREEN_DATA_REG);
  outb(SCREEN_DATA_REG, SET_MASKED_BITS(data_reg_val, ~CD & CURSOR_SCAN_LINE_START, cursor_start));

  // Set cursor scan line to cursor end
  outb(SCREEN_ADDR_REG, CURSOR_END_REG_IDX);
  data_reg_val = inb(SCREEN_DATA_REG);
  outb(SCREEN_DATA_REG, SET_MASKED_BITS(data_reg_val, CURSOR_SCAN_LINE_END, cursor_end));
}


// Set cursor position to loc indices past the start of video memory.
static void update_cursor(uint16_t loc) {
  outb(SCREEN_ADDR_REG, CURSOR_LOC_LO_REG_IDX);
  outb(SCREEN_DATA_REG, loc & 0xff);
  outb(SCREEN_ADDR_REG, CURSOR_LOC_HI_REG_IDX);
  outb(SCREEN_DATA_REG, (loc >> 8U) & 0xff);
}


// NOTE: Currently unused, might be of value in the future.
uint16_t cursor_loc(void) {
  uint16_t loc;
  outb(SCREEN_ADDR_REG, CURSOR_LOC_LO_REG_IDX);
  loc = inb(SCREEN_DATA_REG);
  outb(SCREEN_ADDR_REG, CURSOR_LOC_HI_REG_IDX);
  return loc | (inb(SCREEN_DATA_REG) << 8U);
}


inline static void draw_char(uint8_t ch, struct screen_position *const pos) {
  if (pos->x >= WIDTH) {
    pos->x = 0;
    pos->y += 1;
  }
  // TODO: Scroll screen contents up when term->y >= HEIGHT
  if (ch >= ' ') {
    // Character is printable.
    vmem[pos->idx++] = pos->attr | ch;
    pos->x += 1;
  }
  else switch(ch) {
    case '\n': // Newline
      pos->idx += WIDTH - pos->x;
      pos->x = 0;
      pos->y += 1;
      break;
    case '\r': // Carriage return
      pos->idx -= pos->x;
      pos->x = 0;
      break;
    case '\b': // Backspace
      if (pos->x > 0) {
        pos->idx -= 1;
        pos->x -= 1;
      }
      break;
  }
  update_cursor(pos->idx);
}


// Wipe screen contents by setting every character to space.
// Additionally, set the foreground and background color of every cell.
static void clear_screen(uint16_t attr) {
  uint16_t idx;
  uint16_t cell = attr | ' ';
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      idx = i * WIDTH + j;
      vmem[idx] = cell;
    }
  }
}


// Set screen position ensuring row and column numbers are capped at HEIGHT and WIDTH, respectively.
void set_screen_pos(struct screen_position *const pos, int row, int col) {
  pos->x = col >= WIDTH ? WIDTH : col;
  pos->y = row >= HEIGHT ? HEIGHT : row;
  pos->idx = pos->x * WIDTH + pos->y;
}


// Set the foreground and background color to use when drawing characters. The attribute value is
// shifted 8 bits so that it can be combined with a character with just a bitwise OR.
void set_colors(struct screen_position *const pos, int fg, int bg) {
  pos->attr = ((bg << 4U) | fg) << 8U;
}


// Draw null-terminated string.
void print(struct screen_position *const pos, const char *str) {
  while (*str) {
    draw_char(*str++, pos);
  }
}


void init_screen(struct screen_position *const pos) {
  clear_screen(pos->attr);
  enable_cursor(0, 15);
  update_cursor(0);
}

