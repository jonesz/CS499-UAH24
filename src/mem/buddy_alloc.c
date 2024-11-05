/* buddy_alloc.c; Ethan Jones <erj0005@uah.edu> */
#include "mem/buddy_alloc.h"
#include <stdint.h>

typedef struct _block_t {
  uint32_t addr_beg;
  uint32_t addr_end;
  uint8_t alloc;
  struct _block_t *left;
  struct _block_t *right;
} block_t;

static block_t allocator;

static void *_internal_alloc(block_t *allocator, size_t sz);
void _internal_free(block_t *allocator, void *ptr);

void buddy_alloc_init(uint32_t addr_beg, uint32_t addr_end) {
  allocator.addr_beg = addr_beg;
  allocator.addr_end = addr_end;
  allocator.alloc = 0;
  allocator.left = NULL;
  allocator.right = NULL;
}

void *buddy_alloc(size_t sz) { return _internal_alloc(&allocator, sz); }

void *_internal_alloc(block_t *allocator, size_t sz) {
  // If the block is already allocated, return NULL.
  if (allocator->alloc == 1) {
    return NULL;
  }

  // If we can't fit the allocation in this buffer, return NULL.
  size_t mem_available = allocator->addr_end - allocator->addr_beg;
  if (sz > mem_available) {
    return NULL;
  }

  // If this block has been split, attempt to allocate them with the splits.
  void *recur = NULL;
  if (allocator->left != NULL) {
    recur = _internal_alloc(allocator->left, sz);
    if (recur != NULL) {
      return recur;
    }
  }

  if (allocator->right != NULL) {
    recur = _internal_alloc(allocator->right, sz);
    if (recur != NULL) {
      return recur;
    }
  }

  // Attempt to split the block into two...
  // HDR |               MEM
  // HDR (LEFT: HDR | MEM) (RIGHT: HDR | MEM)
  if (allocator->left == NULL && allocator->right == NULL &&
      ((mem_available / 2) - (sizeof(block_t) * 2) > sz)) {
    allocator->left = (block_t *)allocator->addr_beg;
    allocator->left->addr_beg = allocator->addr_beg + sizeof(block_t);
    allocator->left->addr_end =
        allocator->left->addr_beg + ((mem_available / 2) - sizeof(block_t));
    allocator->left->alloc = 0;
    allocator->left->left = NULL;
    allocator->left->right = NULL;

    allocator->right = (block_t *)allocator->left->addr_end;
    allocator->right->addr_beg = allocator->left->addr_end + sizeof(block_t);
    allocator->right->addr_end = allocator->addr_end;
    allocator->right->alloc = 0;
    allocator->right->left = NULL;
    allocator->right->right = NULL;

    return _internal_alloc(allocator->left, sz);
  } else {
    allocator->alloc = 1;
    return (void *)allocator->addr_beg;
  }
}

void buddy_alloc_free(void *ptr) { _internal_free(&allocator, ptr); }

void _internal_free(block_t *allocator, void *ptr) {
  if (allocator->addr_beg == (uint32_t)ptr) {
    allocator->alloc = 0;
    return;
  }

  if (allocator->left != NULL && allocator->right != NULL) {
    _internal_free(allocator->left, ptr);
    _internal_free(allocator->right, ptr);
  }

  if (allocator->left->alloc == 0 && allocator->right->alloc == 0) {
    allocator->left = NULL;
    allocator->right = NULL;
  }
}
