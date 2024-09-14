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

  for (int i = 31; i >= 0; i--) {
    uint32_t x = boot_info->flags & ((uint32_t)1 << i);
    char c[2] = {'0', 0};
    c[0] += !!x;
    term_write(c);
  }
  term_write("\n");
  for (int i = 31; i >= 0; i--) {
    uint32_t x = boot_info->cmd_line & ((uint32_t)1 << i);
    char c[2] = {'0', 0};
    c[0] += !!x;
    term_write(c);
  }
  term_write("\n");
  term_write((const char *)boot_info->cmd_line);
}
