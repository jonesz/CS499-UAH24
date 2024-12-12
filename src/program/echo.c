#include "libc/string.h"
#include "program/program.h"
#include "syscalls/syscalls.h"

static void printf(char *buf) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = strlen(buf);
  send(&msg, STDOUT);
}

__attribute__((cdecl)) int echo_main(int argc, char **argv) {

  for (uint32_t i = 1; i < argc; i++) {
    printf(argv[i]);

    // NOTE/TODO(BP): Apparently saving the loop index with no less than 3
    // volatile variables causes this to just work properly.
    // Fix this properly
    volatile uint32_t x;
    volatile uint32_t y;
    volatile uint32_t z;
    x = i;
    y = x;
    z = y;
    i = z;
  }

  return 0;
}
