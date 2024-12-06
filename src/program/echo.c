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

__attribute__((cdecl)) int echo_main(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    printf(argv[i]);
  }

  return 0;
}
