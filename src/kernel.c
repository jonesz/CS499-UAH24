/** src/kernel.c */
#include "term.h"

void kernel_main(void) {
  term_init();
  term_writeline("MiniOS Kernel Team #1");
  term_writeline("Testing");
}
