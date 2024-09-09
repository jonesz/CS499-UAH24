#include "kmalloc.h"
#include "term.h"
#include <stdbool.h>
#include <stddef.h>

struct Block {
  struct Block *next; // Next block
  bool used;          // Is the block available?
  size_t size;        // Size of the block
  size_t pages;       // Amount of pages in this block
} Block;

struct Block *memStart = NULL; // Begining of memory
static size_t pageSize =
    4096; // liballoc used this value don't know if it is good or not

void *kmalloc(size_t size) {
  // char s[3] = {0, '\n', '\0'};
  // *s = size % 10 + '0';
  // term_write(s);

  // First time calling kmalloc
  if (memStart == NULL) {
    memStart = newPage(size);
    if (memStart == NULL)
      term_err("New page not written");
    else
      term_write_color("New page written", VGA_COLOR_GREEN);
  }
}


//Most of this is refactored from liballoc (https://github.com/blanham/liballoc/blob/master/liballoc_1_1.c)
static struct Block *newPage(size_t size) {
  size_t pages;
  struct Block *block;

  pages = size + sizeof(struct Block);

  if ((pages % pageSize) == 0)
    pages = pages / (pageSize);
  else
    pages = pages / (pageSize) + 1;
  if (pages < pageSize)
    pages = pageSize;

  // block = (struct block*)alloc(st);

  block->next = NULL;
  block->used = false;
  block->pages = pages;
  block->size = pages * pageSize;

  return block;
}