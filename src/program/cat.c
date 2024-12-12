#include "program/program.h"
#include "syscalls/syscalls.h"
#include "libc/string.h"

static uint32_t read(char *buf, size_t len, uint32_t self_pid) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = len;
  uint32_t result = 1;
  while (result) {
    result = recv(&msg, self_pid);
  }

  return msg.length;
}

__attribute__((cdecl)) int cat_main(int argc, char **argv) {
  uint32_t p = pid();
  
  msg_t msg = {0};
  msg.data = argv[1];
  msg.length = strlen(argv[1]);
  msg.sender = p;
  send(&msg, DUMB_FS_PID);

  char buf[MSG_T_MAX] = {0};
  if (read(buf, MSG_T_MAX, p)) {
    msg.data = buf;
    msg.length = strlen(buf);
    send(&msg, STDOUT);
  }

  return 0;
}
