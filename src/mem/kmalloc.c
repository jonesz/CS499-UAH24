#include "mem/kmalloc.h"
#include "vid/term.h"
#include <stdbool.h>
#include <stddef.h>

#define DEBUG

struct Block {
  size_t size;        // Size of the block
  bool used;          // Is this block being used
  struct Block *next; // Next block in the list
} Block;

struct Block *root = NULL; // Start of the memory
size_t usedMemory = 0;     // Amount of memory we have used so far
size_t minSize = 8; // Minimum amout of memory to give (No idea a good value)
size_t totalMemory = 10240; // To begin with we will assume that kmalloc is
                            // given X amount of memory to work with

struct Block *newBlock(size_t size) {
  if (usedMemory >= totalMemory) {
#ifdef DEBUG
    term_err("Out of usable memory\n");
#endif
    return NULL; // We are out of memory
  }

  size_t blockSize = size; // Testing value I have no idea what a good value is
  struct Block *block = (struct Block *)(usedMemory + 1);
  block->size = blockSize;
  block->used = false;
  block->next = NULL;

  usedMemory += block->size + sizeof(struct Block);

#ifdef DEBUG
  term_format("New block created at: %x\n", &block);
#endif

  return block;
}

void *kmalloc(size_t size) {
  if (size < minSize)
    size = minSize;
  if (root == NULL) {
    root = newBlock(size);
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
      term_format("Total used memory so far: %x\n", &usedMemory);
#endif
      current->used = true;
      return (void *)((size_t)current + sizeof(struct Block));
    }
    if (current->next == NULL) {
      current->next = newBlock(size);
    }
    current = current->next;
  }
  return NULL; // Out of memory
}

void kfree(void *ptr) {
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
