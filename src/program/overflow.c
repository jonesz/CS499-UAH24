#include "libc/string.h"
#include "program/program.h"
#include "syscalls/syscalls.h"
#include "vid/term.h"

static void printf(char *buf) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = strlen(buf);
  send(&msg, STDOUT);
}

uint32_t str_toint(char *s) {
  uint32_t parsing = 1;
  uint32_t i = 0;
  uint32_t sum = 0;

  while (parsing) {

    uint8_t x = s[i];
    x -= '0';
    if (x > 9) {
      parsing = 0;
    } else {
      sum *= 10;
      sum += x;
    }
    i++;
  }
  return sum;
}
__attribute__((cdecl)) int overflow_main(int argc, char **argv) {

  uint32_t def = 32;
  uint32_t x = 0;
  if (argc > 1) {
    x = str_toint(argv[1]);
  }
  if (x == 0) {
    x = def;
  }
  volatile uint32_t idx = 1;
  uint32_t overflows = 0;
  while (overflows < x) {

    if (idx == 0) {
      printf("Overflowed\n");
      overflows += 1;
    }
    idx = (idx + 1) & 0x7FFFFFF;
  }
  return 0;
}