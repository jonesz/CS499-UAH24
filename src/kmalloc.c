#include "kmalloc.h"
#include "term.h"
#include <stdbool.h>
#include <stddef.h>

#define DEBUG

struct Block {
  size_t size;        // Size of the block
  bool used;          // Is this block being used
  struct Block *next; // Next block in the list
} Block;

struct Block *root = NULL;  // Start of the memory
size_t usedMemory = 0;      // Amount of memory we have used so far
size_t totalMemory = 10240; // To begin with we will assume that kmalloc is
                            // given X amount of memory to work with

struct Block *kmallocNew(size_t size) {
  if (usedMemory >= totalMemory) {
#ifdef DEBUG
    term_err("Out of usable memory\n");
#endif
    return NULL; // We are out of memory
  }

  size_t blockSize = size; // Testing value I have no idea what a good value is
  struct Block *block = (struct Block *)(usedMemory + blockSize);
  block->size = blockSize;
  block->used = false;
  block->next = NULL;

  usedMemory += block->size + sizeof(struct Block);

  return block;
}

void *kmalloc(size_t size) {
  if (root == NULL) {
    root = kmallocNew(size);
    if (root == NULL) {
#ifdef DEBUG
      term_err("kmalloc did not initialize\n");
#endif
      return NULL;
    }
#ifdef DEBUG
    term_write_color("kmalloc initialized\n", VGA_COLOR_GREEN);
#endif
  }

  struct Block *current = root;
  while (current != NULL) {
    if (!current->used) {
#ifdef DEBUG
      term_writeline("Free block found");
      size_t *output;
      *output = usedMemory;
      term_format("Total used memory so far: %x\n", output);
#endif
      current->used = true;
      return (void *)((size_t)current + sizeof(struct Block));
    }
    if (current->next == NULL) {
      current->next = kmallocNew(size);
    }
    current = current->next;
  }
  return NULL; // Out of memory
}

void free(void *ptr) {
  if (root == NULL) {
#ifdef DEBUG
    term_err("kmalloc has not been initialized yet\n");
#endif
    return;
  }

  struct Block *current = root;
#ifdef DEBUG
  term_format("Looking for %x in kmalloc list\n",
              (void *)((size_t)current + sizeof(struct Block)));
#endif
  while (current != NULL) {
    if ((void *)((size_t)current + sizeof(struct Block)) == ptr) {
#ifdef DEBUG
      term_format("Freed block at %x\n", &current);
#endif
      current->used = false;
      return;
    }
    current = current->next;
  }
#ifdef DEBUG
  term_err("Provided pointer was not in kmalloc list\n");
#endif
  return;
}