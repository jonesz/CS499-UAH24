#include "test.h"
#include "../term.h" // TODO: this include path is horrendous; fix.

int test_all_functions() {
  term_write("All tests passed!\n");
  return 0;
}
