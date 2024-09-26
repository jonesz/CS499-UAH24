#include "test.h"
#include "test_kmalloc.h"
#include "../term.h" // TODO: this include path is horrendous; fix.

int test_all_functions() {
  term_write("Testing kmalloc... ");
  if (test_kmalloc()) {
    goto err;
  }

  term_write("All tests passed!\n");
  return 0;

  err:
  term_write("Test failed!");
  return 0;
}
