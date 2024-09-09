/*
 * gdt.c by MiniOs
 */

#include <stdint.h>
typedef struct _GDT {
  uint32_t limit;
  uint32_t base;
  uint8_t access_byte;
  uint8_t flags;
} GDT_Entry;

// Puts a GDT entry given by 'source' at location 'target'
// Taken from OsDev.org https://wiki.osdev.org/GDT_Tutorial
void encodeGdtEntry(uint8_t *target, GDT_Entry source) {
  // Check the limit to make sure that it can be encoded
  if (source.limit > 0xFFFFF) {
    return;
  }

  // Encode the limit
  target[0] = source.limit & 0xFF;
  target[1] = (source.limit >> 8) & 0xFF;
  target[6] = (source.limit >> 16) & 0x0F;

  // Encode the base
  target[2] = source.base & 0xFF;
  target[3] = (source.base >> 8) & 0xFF;
  target[4] = (source.base >> 16) & 0xFF;
  target[7] = (source.base >> 24) & 0xFF;

  // Encode the access byte
  target[5] = source.access_byte;

  // Encode the flags
  target[6] |= (source.flags << 4);
}

// Should be called from boot.asm with the pointer to the GDT passed in
// NOTE(Britton): Assumes GDT has 6 entries available and sets up each required
// segment to cover the entire address space
void set_up_gdt(uint8_t *target) {
  GDT_Entry null_segment = {0};
  GDT_Entry kernel_code_segment = {0};
  GDT_Entry kernel_data_segment = {0};
  GDT_Entry user_code_segment = {0};
  GDT_Entry user_data_segment = {0};
  GDT_Entry task_state_segment = {0};

  kernel_code_segment.limit = 0xFFFFF;
  // Set Present, ring 0 privelege, non-system, executable, non-conforming,
  // readable, and not-accessed bit fields
  kernel_code_segment.access_byte = 0b10011010;
  // Set page granularity and size bit fields
  kernel_code_segment.flags = 0b1100;

  kernel_data_segment.limit = 0xFFFFF;
  // Set Present, ring 0 privelege, non-system, non-executable, non-conforming,
  // writable, and not-accessed bit fields
  kernel_data_segment.access_byte = 0b10010010;
  // Set page granularity and size bit fields
  kernel_data_segment.flags = 0b1100;

  user_code_segment.limit = 0xFFFFF;
  // Set Present, ring 3 privelege, non-system, executable, non-conforming,
  // readable, and not-accessed bit fields
  user_code_segment.access_byte = 0b11111010;
  // Set page granularity and size bit fields
  user_code_segment.flags = 0b1100;

  user_data_segment.limit = 0xFFFFF;
  // Set Present, ring 3 privelege, non-system, non-executable, non-conforming,
  // writable, and not-accessed bit fields
  user_data_segment.access_byte = 0b11110010;
  // Set page granularity and size bit fields
  user_data_segment.flags = 0b1100;

  // TODO(Britton): Task state segment

  encodeGdtEntry(target, null_segment);
  encodeGdtEntry(target + 0x8, kernel_code_segment);
  encodeGdtEntry(target + 0x10, kernel_data_segment);
  encodeGdtEntry(target + 0x18, user_code_segment);
  encodeGdtEntry(target + 0x20, user_data_segment);
  encodeGdtEntry(target + 0x28, task_state_segment);
}
