#include "test/test.h"
#include "test/test_kmalloc.h"
#include "vid/term.h"

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
