#include "ipc/ipc.h"
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
  uint8_t buf[256];
  ringbuffer_t rb;
  ringbuffer_init(&rb);

  assert(ringbuffer_read(&rb, buf) == 0);
  assert(ringbuffer_write_bytes(&rb, "hello", strlen("hello")) == 0);
  for (int i = 0; i < strlen("hello"); i++) {
    assert(ringbuffer_read(&rb, buf) == 0);
    printf("%c", buf[0]);
  }

  assert(ringbuffer_read(&rb, buf) == 1);
  printf("\nTests succesful.\n");
  return 0;
}
