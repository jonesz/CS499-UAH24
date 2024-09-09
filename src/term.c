/**
 * TODO: A good bit of this file was taken off OSDev barebones, thus it should
 * be rewritten to some degree.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// VGA text mode buffer base pointer
#define VGA_TEXT_MODE_BUFFER (uint16_t *)0xB8000

static const size_t VGA_WIDTH = 80;  // Width of the screen.
static const size_t VGA_HEIGHT = 25; // Height of the screen.

size_t term_row;    // Current row the terminal is at
size_t term_col;    // Current column the terminal is at
uint16_t *term_buf; // Terminal buffer

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

// Scrolls the terminal down by scroll_lines lines
// term_scroll(0) does nothing
// NOTE (Britton): All the text from lines 0-scroll_lines is lost
void term_scroll(uint32_t scroll_lines) {
  if (scroll_lines >= VGA_HEIGHT) {
    scroll_lines = 1;
  }
  uint16_t *term_buf_line_2 = term_buf + VGA_WIDTH * scroll_lines;
  for (size_t i = 0; i < (VGA_HEIGHT - scroll_lines + 1) * VGA_WIDTH; i++) {
    *(term_buf + i) = *(term_buf_line_2 + i);
  }
}

void term_init() {
  // TODO: Apparently this is deprecated in UEFI.
  term_buf = VGA_TEXT_MODE_BUFFER;
  term_row = 0;
  term_col = 0;

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t idx = y * VGA_WIDTH + x;
      term_buf[idx] = vga_entry(' ', VGA_COLOR_WHITE);
    }
  }
}

// Write a single char to the buffer.
void term_write_char(const char *c) {
  if (++term_col == VGA_WIDTH) {
    term_row++;
    term_col = 0;
  }

  // Scroll the terminal
  if (term_row + 1 == VGA_HEIGHT) {
    term_scroll(1);
    term_row--;
    term_col = 1;
  }

  if (*c == '\n') {
    term_row++;
    term_col = 0;
  }

  size_t term_pos = term_row * VGA_WIDTH + term_col;
  term_buf[term_pos] = vga_entry(*c, VGA_COLOR_WHITE);
}

void term_write_color(const char *s, const uint8_t color) {
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) {

    if (++term_col == VGA_WIDTH) {
      term_row++;
      term_col = 0;
    }
    // Scroll the terminal
    if (term_row + 1 == VGA_HEIGHT) {
      term_scroll(1);
      term_row--;
      term_col = 1;
    }

    if (s[i] == '\n') {
      term_row++;
      term_col = 0;
      continue;
    }

    size_t term_pos = term_row * VGA_WIDTH + term_col;
    term_buf[term_pos] = vga_entry(s[i], color);
  }
}

// Write out a string to the terminal buffer.
void term_write(const char *s) { term_write_color(s, VGA_COLOR_WHITE); }

// Write an error to the terminal buffer
// TODO: Anything besides just being red
void term_err(const char *s) { term_write_color(s, VGA_COLOR_RED); }

// Write a warning to the terminal buffer
void term_warn(const char *s) {
  // "Light brown" is actually yellow
  term_write_color(s, VGA_COLOR_LIGHT_BROWN);
}

// Write to the terminal buffer and end with a newline
// This seems kind of redundent but I added it anyway
// Feel free to remove
void term_writeline(const char *s) {
  term_write_color(s, VGA_COLOR_WHITE);
  term_col = 0;
  term_row++;
}

enum format_specifier {
  HEX = 0,
  STRING = 1,
  SIGNED = 2,
  UNSIGNED = 3,
};

/// A single variable `term_write` function in the style of `printf`.
void term_format(const char *s, void *ptr) {
  char *start = NULL;
  enum format_specifier f;

  // Attempt to find %x.
  start = strstr(s, "%x");
  if (start) {
    f = HEX;
    goto write;
  }

  // %s
  start = strstr(s, "%s");
  if (start) {
    f = STRING;
    goto write;
  }

  // %d
  start = strstr(s, "%d");
  if (start) {
    f = SIGNED;
    goto write;
  }

  // %u
  start = strstr(s, "%u");
  if (start) {
    f = UNSIGNED;
    goto write;
  }

  return;

write:
  while (s != start) {
    term_write_char(s++);
  }

  if (f == HEX) {
    char hex_digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    unsigned int h = *(unsigned int *)ptr;

    term_write("0x");
    for (int i = 0; i < 8; i++) {
      unsigned tmp = (h >> (4 * i)) & 0xF;
      term_write_char(&hex_digits[tmp]);
    }
  } else if (f == STRING) {
    term_write((const char *)ptr);
  } else if (f == SIGNED) {
    // TODO
  } else if (f == UNSIGNED) {
    // TODO
  }

  s += 2;
  term_write(s);
}
