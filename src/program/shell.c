#include "syscalls/syscalls.h"
#include "libc/string.h"
#include "program/program.h"

static void printf(char *buf) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = strlen(buf);
  send(&msg, STDOUT);
}

static uint32_t read(char *buf, size_t len) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = len;
  return recv(&msg, STDIN);
}

int shell_main(int argc, char **argv) {
  char buf[MSG_T_MAX] = {0};
    
  while (1) {
    printf(">");
    if (read(buf, MSG_T_MAX)) {
      if (strcmp("echo", buf) == 0) {
        // FORK and spawn echo with the rest of the parameters.
      }
    } else {
      // DO NOTHING.
    }
  }
}
