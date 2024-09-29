/*
 * ringbuffer.c - A ringbuffer implementation.
 * Ethan Jones <erj0005@uah.edu>
 */

#include "ringbuffer.h"
#include <string.h>

// Initialize the ringbuffer with a `len` length `buf`.
void ringbuffer_init(ringbuffer_t *rb, uint8_t *buf, size_t len) {
  rb->buf = buf;
  rb->len = len;
  rb->cur = 0;

  memset(buf, 0, len);
}

// Write some bytes to the ringbuffer.
void ringbuffer_write(ringbuffer_t *rb, uint8_t *in, size_t len) {
  for (size_t i = 0; i < len; i++) {
    rb->cur = (rb->cur + 1) %
              rb->len; // Avoid some potential (?) weird overflow behavior.
    rb->buf[rb->cur] = *in++;
  }
}
