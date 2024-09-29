#ifndef UTIL_RINGBUFFER_H
#define UTIL_RINGBUFFER_H
#include <stddef.h>
#include <stdint.h>

typedef struct _ringbuffer_t {
  uint8_t *buf;
  size_t len;
  size_t cur;
} ringbuffer_t;

void ringbuffer_init(ringbuffer_t *rb, uint8_t *buf, size_t len);
void ringbuffer_write(ringbuffer_t *rb, uint8_t *in, size_t len);

#endif // UTIL_RINGBUFFER_H
