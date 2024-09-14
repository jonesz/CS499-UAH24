/** src/kernel.c */
#include "multiboot.h"
#include "term.h"

void kernel_main(BootInformation *boot_info) {
  term_init();
  term_write("MiniOS Kernel Team #1\n");
  term_writeline("Testing writeline");
  term_err("This is an error\n");
  term_warn("This is a warning\n");

  term_format("This is a format string: %s.\n", "Hello");
  unsigned int tmp = 0xFFFFAAAA;
  void *loc = kernel_main;
  term_format("This is a format string hex: %x\n", &tmp);
  term_format("This is a format string hex: %x\n", &loc);
  term_write((const char *)boot_info->cmd_line);
  term_write("\n");

}
