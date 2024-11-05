#ifndef MEM_BUDDY_ALLOC_H
#define MEM_BUDDY_ALLOC_H
#include <stdint.h>
#include <stddef.h>

void buddy_alloc_init(uint32_t addr_beg, uint32_t addr_end);
void *buddy_alloc(size_t sz);
void buddy_alloc_free(void *ptr);

#endif // MEM_BUDDY_ALLOC_H
