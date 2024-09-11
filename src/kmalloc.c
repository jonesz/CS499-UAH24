#include "kmalloc.h"
#include "term.h"
#include <stdbool.h>
#include <stddef.h>

// Note: somehow calling term_* makes root NULL so
// until that is fixed don't uncomment this
//  #define DEBUG

struct Block {
  size_t size;        // Size of the block
  bool used;          // Is this block being used
  struct Block *next; // Next block in the list
} Block;

struct Block *root = NULL; // Start of the memory

// To begin with we will assume that kmalloc is given X amount of memory to work
// with
struct Block *kmallocNew() {
  struct Block *block;

  block->size = 1024; // Testing value I have no idea what a good value is
  block->used = false;
  block->next = NULL;

  return block;
}

void *kmalloc(size_t size) {
  if (root == NULL) {
    root = kmallocNew();
    if (root == NULL) {
#ifdef DEBUG
      term_err("kmalloc did not initialize");
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
#endif
      current->used = true;
      return (void *)((size_t)current + sizeof(struct Block) + current->size);
    }
    current = current->next;
  }
}