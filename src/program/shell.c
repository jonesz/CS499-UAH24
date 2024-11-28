#include "syscalls/syscalls.h"
#include "libc/string.h"

int shell_main(int argc, char **argv) {
  char buf[256];
  // write()
  
  while (1) {
    // TODO: Write a RECV instruction.
    // recv();

    // TODO: Determine
    if (strcmp("echo", buf) == 0) {
      
    } else {
      // TODO: Construct a write syscall.
      // write();
    }
  }
}
