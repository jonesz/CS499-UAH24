/**
 * `mem/buddy_alloc.c`, Ethan Jones <erj0005@uah.edu>
 **/
#include "mem/buddy_alloc.h"

// TODO: all of this code is busted; don't use it.

typedef struct _node_t {
  uint32_t offset;
  size_t sz;
  uint8_t allocated;
  struct _node_t *left;
  struct _node_t *right;
} node_t;

static node_t allocator;

static void *internal_alloc(node_t *node, size_t req);
static void internal_free(node_t *node, void *ptr);

// Initialize the allocator.
void buddy_alloc_init(uint32_t offset, size_t sz) {
  allocator.offset = offset;
  allocator.sz = sz;
  allocator.allocated = 0;
  allocator.left = NULL;
  allocator.right = NULL;
  return;
}

void *buddy_alloc(size_t sz) { return internal_alloc(&allocator, sz); }
void buddy_alloc_free(void *ptr) { internal_free(&allocator, ptr); }

static void *internal_alloc(node_t *node, size_t req) {
  // The node can't fit the request, or it's already allocated, or if it's
  // already been split; return NULL.
  if (req > node->sz || node->allocated != 0 ||
      (node->left != NULL && node->right != NULL)) {
    return NULL;
  }

  // If the node has been split, attempt to allocate within those splits.
  if (node->left != NULL && node->right != NULL) {
    void *left_result = internal_alloc(node->left, req);
    if (left_result != NULL) {
      return left_result;
    }

    void *right_result = internal_alloc(node->right, req);
    if (right_result != NULL) {
      return right_result;
    }
  }

  // Split the node, and then attempt to allocate.
  node_t *left = (node_t *)node->offset;
  node_t *right = (node_t *)node->offset + sizeof(node_t);
  node->left = left;
  node->right = right;

  size_t rem = node->sz - (sizeof(node_t) * 2);
  left->offset = node->offset + sizeof(node_t) * 2;
  left->sz = rem / 2;
  left->left = NULL;
  left->right = NULL;

  right->offset = (node->offset + sizeof(node_t) * 2) + (rem / 2);
  right->sz = rem / 2;
  right->left = NULL;
  right->right = NULL;

  // Attempt to allocate on the left side, if it fails then reset the left and
  // right pointers back to NULL.
  void *left_result = internal_alloc(left, req);
  if (left_result != NULL) {
    return left_result;
  }

  // If that wasn't succesful, allocate within this chunk.
  node->left = NULL;
  node->right = NULL;
  node->allocated = 1;
  return (void *)node->offset;
}

static void internal_free(node_t *node, void *ptr) {
  if (node == NULL) {
    return;
  }

  if (node->offset == (uint32_t)ptr) {
    node->allocated = 0;
    return;
  } else {
    internal_free(node->left, ptr);
    internal_free(node->right, ptr);

    // To merge the child blocks, we need...
    // 1. The left and right of this node to be non NULL.
    // 2. Both of those children to not be allocated.
    // 3. Both of those children to not be split themselves.
    if (node->left != NULL && node->right != NULL &&
        node->left->allocated == 0 && node->right->allocated == 0 &&
        node->left->left == NULL && node->left->right == NULL &&
        node->right->left == NULL && node->right->right == NULL) {
      node->left = NULL;
      node->right = NULL;
    }
  }
}
