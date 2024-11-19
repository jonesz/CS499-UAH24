#ifndef MEM_FIXED_ALLOC_H
#define MEM_FIXED_ALLOC_H
#include <stdint.h>
#include <stddef.h>

void fixed_alloc_init(uint32_t offset, size_t sz, size_t block_sz);
void *fixed_alloc(size_t sz);
void fixed_free(void *ptr);

#endif // MEM_FIXED_ALLOC_H
