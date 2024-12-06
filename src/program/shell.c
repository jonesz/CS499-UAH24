#include "libc/string.h"
#include "mem/bump_alloc.h"
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

// TODO: Make this handle quotations.
static uint32_t to_argc(char *buf) {
  // uint32_t argc = 1;
  // for (int i = 0; i < strlen(buf); i++) {
  //   if (buf[i] == ' ') {
  //     argc++;
  //   }
  // }

  // return argc;
  // TODO: This is a really dumb hack.
  switch (strlen(buf)) {
  case 0:
    return 0;
  case 1:
    return 1;
  default:
    return 2;
  }
}

// TODO: Holy crap this is a dumb hack.
static char **to_argv(char *buf) {
  size_t argc = to_argc(buf);
  if (argc == 0 || argc == 1) {
    return NULL;
  }

  char **argv = bump_alloc(sizeof(char *) * argc);

  size_t buf_len = strlen(buf);
  for (int i = 0; i < buf_len; i++) {
    if (buf[i] == ' ') {

      // Split the argv into two chunks.
      argv[0] = bump_alloc(sizeof(char) * i);
      for (int j = 0; j < i; j++) {
        argv[0][j] = buf[j];
      }
      argv[0][i] = '\0';

      i++; // Skip the ' ' value.

      // +1 to catch the '\0'.
      argv[1] = bump_alloc(sizeof(char) * ((buf_len - i) + 1));
      for (int j = i; j < buf_len + 1; j++) {
        argv[1][j - i] = buf[j];
      }

      return argv;
    }
  }

  return NULL;
}

int shell_main(int argc, char **argv) {
  char buf[MSG_T_MAX] = {0};
  printf("Dropping into a shell...\n");

  while (1) {
    printf("\n> ");
    if (read(buf, MSG_T_MAX)) {
      if (strncmp("echo", buf, 4) == 0) {
        spawn((uint32_t)&echo_main, to_argc(buf), to_argv(buf));

        // Free the ARGV arr + the individual strings.
        for (int i = 0; i < to_argc(buf) + 1; i++) {
          bump_free(NULL);
        }

      } else {
        printf("Unrecognized command.");
      }
    }
  }
}
