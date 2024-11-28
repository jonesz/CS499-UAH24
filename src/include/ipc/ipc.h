#ifndef IPC_IPC_H
#define IPC_IPC_H
#include <stdint.h>
#include <stddef.h>

#define RINGBUFFER_SIZE 2048

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

void ringbuffer_init(ringbuffer_t *rb);
int ringbuffer_write(ringbuffer_t *rb, uint8_t byte);
int ringbuffer_write_bytes(ringbuffer_t *rb, uint8_t *buf, size_t len);
int ringbuffer_read(ringbuffer_t *rb, uint8_t *buf);
void ipc_init();

#endif // IPC_IPC_H

