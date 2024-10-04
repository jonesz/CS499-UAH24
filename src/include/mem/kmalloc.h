#ifndef MEM_KMALLOC_H
#define MEM_KMALLOC_H

#include <stddef.h>

void *kmalloc(size_t size);
void free(void *ptr);

#endif // KMALLOC_H
