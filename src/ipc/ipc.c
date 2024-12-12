/**
 * `ipc/ipc.c`, Ethan Jones <erj0005@uah.edu>.
 */

#include "ipc/ipc.h"
#include "sched/sched.h"
#include <stddef.h>
#include <stdint.h>

/*
 * A ringbuffer that won't let the `read_ptr` advance past the `write_ptr` (and
 * by the same token, won't allow the `write_ptr` to advance past the
 * `read_ptr`); as a result, we can tell when the buffer has mem to be read.
 */

// Store the last PID that wrote the `process_buffers`.
uint32_t last_writer[MAX_PROCESSES];
ringbuffer_t process_buffers[MAX_PROCESSES];
ringbuffer_t stdin;

void ringbuffer_init(ringbuffer_t *rb) {
  rb->r_ptr = 0;
  rb->w_ptr = 0;
  return;
}

int ringbuffer_write(ringbuffer_t *rb, uint8_t byte) {
  if ((rb->w_ptr + 1) % RINGBUFFER_SIZE == rb->r_ptr) {
    return 1; // The buffer is full.
  }

  rb->buf[rb->w_ptr] = byte;
  rb->w_ptr = (rb->w_ptr + 1) % RINGBUFFER_SIZE;
  return 0;
}

int ringbuffer_write_bytes(ringbuffer_t *rb, uint8_t *buf, size_t len) {
  for (int i = 1; i < len + 1; i++) {
    if ((rb->w_ptr + i) % RINGBUFFER_SIZE == rb->r_ptr) {
      return 1; // The buffer is full and can't hold what needs to be written.
    }
  }

  for (int i = 0; i < len; i++) {
    ringbuffer_write(rb, *buf++);
  }

  return 0;
}

int ringbuffer_read(ringbuffer_t *rb, uint8_t *buf) {
  if (rb->r_ptr == rb->w_ptr) {
    return 1; // There's nothing to read from the buffer.
  }

  *buf = rb->buf[rb->r_ptr];
  rb->r_ptr = (rb->r_ptr + 1) % RINGBUFFER_SIZE;
  return 0;
}

ringbuffer_t process_buffers[MAX_PROCESSES];
ringbuffer_t ipc_stdin;

void ipc_init() {
  for (int i = 0; i < MAX_PROCESSES; i++) {
    ringbuffer_init(&process_buffers[i]);
  }

  ringbuffer_init(&ipc_stdin);
}
