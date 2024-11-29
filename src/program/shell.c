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
  uint32_t result = 1;
  while (result) {
    result = recv(&msg, STDIN);
  }
  printf("Returning from READ.\n");

  return 1;
}

int shell_main(int argc, char **argv) {
  char buf[MSG_T_MAX] = {0};
    
  while (1) {
    printf("> ");
    if (read(buf, MSG_T_MAX)) {
      printf("Ain't that something.\n");
      if (strcmp("echo", buf) == 0) {
        // FORK and spawn echo with the rest of the parameters.
        printf("Spawning echo...\n");
      }
    } else {
    }
  }
}
