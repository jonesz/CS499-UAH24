#include "program/program.h"
#include "syscalls/syscalls.h"
#include "libc/string.h"

static void printf(char *buf) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = strlen(buf);
  send(&msg, STDOUT);
}

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
  if (send(&msg, DUMB_FS_PID)) {
    printf("Unable to send...\n");
  }

  char buf[MSG_T_MAX] = {0};
  if (read(buf, MSG_T_MAX, p)) {
    printf(buf);
  }

  printf("Exiting...\n");

  return 0;
}
