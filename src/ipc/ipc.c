/**
 * `ipc/ipc.c`, Ethan Jones <erj0005@uah.edu>.
 */

#include "sched/sched.h"
#include <stdint.h>
#include <stddef.h>

#define RINGBUFFER_SIZE 1024

/*
 * A ringbuffer that won't let the `read_ptr` advance past the `write_ptr` (and by the same token,
 * won't allow the `write_ptr` to advance past the `read_ptr`); as a result, we can tell when the
 * buffer has mem to be read.
 */

typedef struct _ringbuffet_t {
  uint8_t buf[RINGBUFFER_SIZE];
  size_t r_ptr;
  size_t w_ptr;
} ringbuffer_t;

static void ringbuffer_init(ringbuffer_t *rb) {
  rb->r_ptr = 0;
  rb->w_ptr = 0;
  return;
}

static int ringbuffer_write(ringbuffer_t *rb, uint8_t byte) {
  if ((rb->w_ptr + 1) % RINGBUFFER_SIZE == rb->r_ptr) {
    return -1; // The buffer is full.
  }

  rb->buf[rb->w_ptr] = byte;
  rb->w_ptr = (rb->w_ptr + 1) % RINGBUFFER_SIZE;
  return 0;
}

static int ringbuffer_read(ringbuffer_t *rb, uint8_t *buf) {
  if (rb->r_ptr == rb->w_ptr) {
    return -1; // There's nothing to read from the buffer.
  }

  *buf = rb->buf[rb->r_ptr];
  rb->r_ptr = (rb->r_ptr + 1) % RINGBUFFER_SIZE;
  return 0;
}
