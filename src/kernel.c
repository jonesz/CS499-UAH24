/** src/kernel.c */
#include "term.h"

void kernel_main(uint32_t in) {
  term_init();
  term_write("MiniOS Kernel Team #1\n");
  term_writeline("Testing writeline");
  term_err("This is an error\n");
  term_warn("This is a warning\n");

  // NOTE(Britton): Debug Code

  // Mask to 'and out' the flag bits from page table entry
  uint32_t mask = ~0xFFF;
  // Move 'in' to the second page table - addressing second meg of memory
  in += 4096;
  // Print the address of page_tables[1]
  for (int i = 31; i >= 0; i--) {
    uint32_t x = in & ((uint32_t)1 << i);
    char c[2] = {'0', 0};
    c[0] += !!x;
    term_write(c);
  }
  term_write("\n");

  // Check The contents of page_tables[1]
  // Bit 5 is 'accessed' and bit 6 is 'dirty' - they are both 0
  for (int i = 31; i >= 0; i--) {
    uint32_t x = *(uint32_t *)in & ((uint32_t)1 << i);
    char c[2] = {'0', 0};
    c[0] += !!x;
    term_write(c);
  }

  // Extract the actual pointer from the page table entry
  uint32_t *ptr = *(uint32_t **)in;
  ptr = (uint32_t *)((uint32_t)ptr & mask);

  // Check what memory we are addressing
  term_write("\n");
  for (int i = 31; i >= 0; i--) {
    uint32_t x = (uint32_t)ptr & ((uint32_t)1 << i);
    char c[2] = {'0', 0};
    c[0] += !!x;
    term_write(c);
  }
  term_write("\n");
  term_write("\n");

  // Modify the data stored at 1MB
  *ptr = 0b101010101010;
  // Recheck contents of page_table[1]
  // Bits 5 and 6 were set - paging is working
  for (int i = 31; i >= 0; i--) {
    uint32_t x = *(uint32_t *)in & ((uint32_t)1 << i);
    char c[2] = {'0', 0};
    c[0] += !!x;
    term_write(c);
  }
  term_write("\n");
  // Check if the memory was actually modified
  for (int i = 31; i >= 0; i--) {
    uint32_t x = *ptr & ((uint32_t)1 << i);
    char c[2] = {'0', 0};
    c[0] += !!x;
    term_write(c);
  }
  term_write("\n");
}
