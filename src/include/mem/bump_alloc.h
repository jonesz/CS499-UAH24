#ifndef MEM_BUMP_ALLOC_H
#define MEM_BUMP_ALLOC_H
#include <stdint.h>
#include <stddef.h>

void bump_alloc_init(uint32_t offset, size_t sz);
void *bump_alloc(size_t sz);
void bump_free(void *ptr);

#endif // MEM_BUMP_ALLOC_H
