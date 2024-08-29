/** src/kernel.c */
#include "term.h"

void kernel_main(void) {
  term_init();
  term_write("MiniOS Kernel Team #1\n");
  term_writeline("Testing writeline");
  term_err("This is an error\n");
  term_warn("This is a warning\n");
  
  // TODO (Britton): Remove this test code for proving that the terminal code scrolls
  uint32_t x = 0;
  while(1) {
    char s[3] = {0, '\n', '\0'};
    *s = x % 10 + '0';
    x++;
    term_write(s);
    for(unsigned int i = 0; i < 0x8fff000; i++) {
    }
  }

}
