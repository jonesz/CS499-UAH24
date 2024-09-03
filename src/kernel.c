/** src/kernel.c */
#include "term.h"

void kernel_main(uint64_t *gdtp) {
  term_init();
  term_write("MiniOS Kernel Team #1\n");
  term_writeline("Testing writeline");
  term_err("This is an error\n");
  term_warn("This is a warning\n");

  // NOTE(Britton): Debug code
  for (int j = 0; j < 6; j++) {
    for (int i = 63; i >= 0; i--) {
      uint64_t x = *gdtp & ((uint64_t)1 << i);
      char c[2] = {'0', 0};
      c[0] += !!x;
      term_write(c);
    }
    term_writeline("\n");
    gdtp += 1;
  }
}
