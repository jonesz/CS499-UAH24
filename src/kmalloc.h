#ifndef KMALLOC_H
#define KMALLOC_H

#include <stddef.h>

void *kmalloc(size_t size);
static struct Block *newPage(size_t size);

#endif // KMALLOC_H