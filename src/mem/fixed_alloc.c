/**
 * `mem/fixed_alloc.c`, Ethan Jones <erj0005@uah.edu>
 **/
#include "mem/fixed_alloc.h"
#include "libc/string.h"

// TODO: There's a chance I want to move this sturcture to the header and
// initialize multiple of these.
typedef struct _alloc_t {
  uint32_t offset;
  size_t sz;
  size_t block_sz;
} alloc_t;

static alloc_t allocator;

// Compute the amount of the offset that will be used as the `header`, which is
// dependent on the total `sz` and `block_sz`.
static size_t header_sz();
// We've computed the header size above, but *this* is the number of actual mem
// blocks we can serve.
static size_t header_bound();

// Initialize the allocator; there are `sz` bytes after `offset` to allocate
// `block_sz` blocks on demand.
void fixed_alloc_init(uint32_t offset, size_t sz, size_t block_sz) {
  allocator.offset = offset;
  allocator.sz = sz;
  allocator.block_sz = block_sz;

  // Wipe the header information; everything is allocated.
  // TODO: This is fucking up.
  // memset((void *)allocator.offset, 0, allocator.sz / allocator.block_sz);

  uint8_t *header = (uint8_t *)offset;
  for (unsigned int i = 0; i < header_sz(); i++) {
    *header++ = 0;
  }

  return;
}

void *fixed_alloc() {
  size_t max_blocks = header_sz();
  size_t bound = header_bound();

  // Attempt to find an allocation...
  uint8_t *header = (uint8_t *)allocator.offset;
  for (unsigned int i = 0; i < bound; i++) {
    if (*header == 1) {
      header++; // Skip over existing allocated chunks.
    } else {
      *header = 1;
      uint32_t a =
          (uint32_t)(allocator.offset + max_blocks + (i * allocator.block_sz));
      return (void *)(allocator.offset + max_blocks + (i * allocator.block_sz));
    }
  }
  return NULL;
}

void fixed_free(void *ptr) {
  // TODO: This is the brain dead way to do this.
  size_t max_blocks = header_sz();
  size_t bound = header_bound();

  uint8_t *header = (uint8_t *)allocator.offset;
  for (unsigned int i = 0; i < bound; i++) {
    if (allocator.offset + max_blocks + (i * allocator.block_sz) ==
        (uint32_t)ptr) {
      *header = 0;
    } else {
      header++;
    }
  }
}

// TODO: This isn't perfect.
// 1. Compute the total number of blocks without the header information.
// 2. Subtract that header size from the size of the existing contigious mem
// block.
// 3. Recompute the number of blocks we can use now; that's the new block total.
static size_t header_sz() { return allocator.sz / allocator.block_sz; }
static size_t header_bound() {
  return (allocator.sz - header_sz()) / allocator.block_sz;
}
