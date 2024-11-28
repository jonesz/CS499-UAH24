#include "syscalls/syscalls.h"
#include "program/program.h"

int echo_main(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    // TODO: Construct a WRITE syscall message to print to screen.
    // write();
  }

  return 0;
}
