/**
 * TODO: A good bit of this file was taken off OSDev barebones, thus it should
 * be rewritten to some degree.
 */
#include <stddef.h>
#include <stdint.h>

static size_t strlen(const char *str);

static const size_t VGA_WIDTH = 80;  // Width of the screen.
static const size_t VGA_HEIGHT = 25; // Height of the screen.

size_t term_row;
size_t term_col;
uint8_t term_color; //unused at the moment
uint16_t *term_buf;

/* TODO: Rewrite this, I took this off of OSDev, so it needs to be sourced or
 * rewritten. */
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

/* TODO: Rewrite this, I took this off of OSDev, so it needs to be sourced or
 * rewritten. */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

/* TODO: Rewrite this, I took this off of OSDev, so it needs to be sourced or
 * rewritten. */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

void term_init() {
  // TODO: Apparently this is deprecated in UEFI.
  term_buf = (uint16_t *)0xB8000;
  term_row = 0;
  term_col = 0;

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t idx = y * VGA_WIDTH + x;
      term_buf[idx] = vga_entry(' ', VGA_COLOR_WHITE);
    }
  }
}

// Write out a string to the terminal buffer.
void term_write(const char *s) {
  size_t len = strlen(s);
  // TODO: This is wrong, but it's proof-of-concept at this point. Fix later.
  // for (size_t i = 0; i < len; i++) {
  //   term_buf[i] = vga_entry(s[i], VGA_COLOR_WHITE);
  // }
  for (size_t i = 0; i < len; i++) {
    
    if(++term_col == VGA_WIDTH) {
      term_row++;
      term_col = 0;
    }
    if(term_row + 1 == VGA_HEIGHT) {
      // TODO: Add scrolling the terminal up a line
    }

    size_t term_pos = term_row * VGA_WIDTH + term_col;
    term_buf[term_pos] = vga_entry(s[i], VGA_COLOR_WHITE);
  }
}

//Write to the terminal buffer and end with a newline
//This seems kind of redundent but I added it anyway
//Feel free to remove
void term_writeline(const char *s) {
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) {
    if(++term_col == VGA_WIDTH) {
      term_row++;
      term_col = 0;
    }
    if(term_row + 1 == VGA_HEIGHT) {
      // TODO: Add scrolling the terminal up a line
    }
    
    size_t term_pos = term_row * VGA_WIDTH + term_col;
    term_buf[term_pos] = vga_entry(s[i], VGA_COLOR_WHITE);
  }
  term_col = 0;
  term_row++;
}

// Return the size of a string.
static size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }

  return len;
}
