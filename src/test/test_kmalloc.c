#include "test/test_kmalloc.h"
#include "mem/buddy_alloc.h"
#include "vid/term.h"
#include <stddef.h>

int test_kmalloc() {
  void *ptr_1 = buddy_alloc(100);
  if (ptr_1 == NULL) {
    term_write("Hello");
    return 1;
  }

  void *ptr_2 = buddy_alloc(200);
  if (ptr_1 == NULL || ptr_1 == ptr_2) {
    term_write("Hello 2");
    return 1;
  }

  buddy_alloc_free(ptr_1);
  buddy_alloc_free(ptr_2);

  return 0;
}
