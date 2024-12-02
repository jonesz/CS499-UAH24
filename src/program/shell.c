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

static uint32_t read(char *buf, size_t len) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = len;
  uint32_t result = 1;
  while (result) {
    result = recv(&msg, STDIN);
  }

  return msg.length;
}

int shell_main(int argc, char **argv) {
  char buf[MSG_T_MAX] = {0};
  printf("Dropping into a shell...\n");

  while (1) {
    printf("\n> ");
    if (read(buf, MSG_T_MAX)) {
      if (strncmp("echo", buf, 4) == 0) {
        printf("Spawning echo...\n");
      } else {
        printf("Unrecognized command.");
      }
    }
  }
}
