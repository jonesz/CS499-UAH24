/**
 * `mem/bump_alloc.c`, Ethan Jones <erj0005@uah.edu>
 **/

#include "mem/bump_alloc.h"
#include "vid/term.h"

// A simple bump allocator.

typedef struct _allocator_t {
  uint32_t offset;
  size_t sz;

  uint32_t cur;
  size_t ctr;
} allocator_t;

static allocator_t allocator;

void bump_alloc_init(uint32_t offset, size_t sz) {
  allocator.offset = offset;
  allocator.sz = sz;

  allocator.cur = 0;
  allocator.ctr = 0;
}

void *bump_alloc(size_t req_sz) {
  if (allocator.cur + req_sz > allocator.offset + allocator.sz) {
    return NULL;
  }

  void *ptr = (void *)allocator.cur;
  allocator.cur += (req_sz + 1);
  allocator.ctr++;
  return ptr;
}

void bump_free(void *ptr) {
  allocator.ctr--;
  if (allocator.ctr == 0) {
    allocator.cur = allocator.offset;
  }
}
