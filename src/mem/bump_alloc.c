/**
 * `mem/bump_alloc.c`, Ethan Jones <erj0005@uah.edu>
 **/

#include "mem/bump_alloc.h"

typedef struct _allocator_t {
  uint32_t offset;
  size_t sz;

  uint32_t cur;
  size_t ctr;
} allocator_t;

static allocator_t allocator;

void bump_alloc_init(uint32_t offset, size_t sz) {
  
}

void *bump_alloc(size_t sz) {
  if (allocator.cur + sz > allocator.offset + allocator.sz) {
    return NULL;
  } 
    void *ptr = (void *)allocator.cur;
    allocator.cur += sz;
    allocator.ctr++;
    return ptr;
}

void bump_free(void *ptr) {
  allocator.ctr--;
  if (allocator.ctr == 0) {
    allocator.cur = allocator.offset;
  }
}
