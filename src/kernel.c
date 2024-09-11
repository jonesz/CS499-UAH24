/** src/kernel.c */
#include "kmalloc.h"
#include "multiboot.h"
#include "term.h"
#include <stddef.h>

void kernel_main(multiboot_info_t *mbd, unsigned int magic) {
  term_init();
  term_write("MiniOS Kernel Team #1\n");
  term_writeline("Testing writeline");
  term_err("This is an error\n");
  term_warn("This is a warning\n");

  /* Make sure the magic number matches for memory mapping*/
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    term_err("invalid magic number!\n");
  }

  /* Check bit 6 to see if we have a valid memory map */
  if (!(mbd->flags >> 6 & 0x1)) {
    term_err("invalid memory map given by GRUB bootloader\n");
  }

  term_format("This is a format string: %s.\n", "Hello");
  unsigned int tmp = 0xFFFFAAAA;
  void *loc = kernel_main;
  term_format("This is a format string hex: %x\n", &tmp);
  term_format("This is a format string hex: %x\n", &loc);

  /* Loop through the memory map and display the values */
  for (size_t i = 0; i < mbd->mmap_length;
       i += sizeof(multiboot_memory_map_t)) {
    multiboot_memory_map_t *mmmt =
        (multiboot_memory_map_t *)(mbd->mmap_addr + i);

    // We don't have printf implemented yet
    // printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
    //     mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
    // term_write("Start Addr: | ");
    // term_write("Length: | ");
    // term_write("Size: | ");
    // term_write("Type: |\n");

    if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
      /*
       * Do something with this memory block!
       * BE WARNED that some of memory shown as availiable is actually
       * actively being used by the kernel! You'll need to take that
       * into account before writing to memory!
       */
    }
  }

  int *test = kmalloc(sizeof(int));
}