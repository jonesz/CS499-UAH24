#include "program/program.h"
#include "syscalls/syscalls.h"
#include "libc/string.h"

static uint32_t read(char *buf, size_t len) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = len;
  uint32_t result = 1;
  while (result) {
    // result = recv(&msg, ); // TODO: Need to figure out what this PID is.
  }

  return msg.length;
}

__attribute__((cdecl)) int cat_main(int argc, char **argv) {
  msg_t msg = {0};
  msg.data = argv[1];
  msg.length = strlen(argv[1]);
  send(&msg, DUMB_FS_PID);

  char buf[MSG_T_MAX] = {0};
  if (read(buf, MSG_T_MAX)) {
    msg.data = buf;
    msg.length = strlen(buf);
    send(&msg, STDOUT);
  }

  return 0;
}
