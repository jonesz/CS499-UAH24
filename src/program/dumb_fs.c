#include "program/program.h"
#include "syscalls/syscalls.h"
#include <stdint.h>
#include <stddef.h>
#include "libc/string.h"
#include "vid/term.h"

static void printf(char *buf) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = strlen(buf);
  send(&msg, STDOUT);
}

#define FW_CFG_PORT_SEL 0x510
#define FW_CFG_PORT_DATA 0x

#define FW_CFG_SIGNATURE 0x0
#define FW_CFG_ID 0x1
#define FW_CFG_FILE_DIR 0x19

typedef struct FWCfgFile {
  uint32_t size;
  uint16_t select;
  uint16_t reserved;
  char name[56];
} FWCfgFile;

void dumb_fs(int argc, char **argv) {
  uint32_t p = pid();
  if (p != DUMB_FS_PID) {
    printf("Not running under correct pid...\n");
  }
  char buf[MSG_T_MAX] = {0};

  msg_t msg_out = {0};
  msg_out.data = buf;
  msg_out.sender = p;

  while (1) {
    // read a request.
    msg_t msg_in = {0};
    msg_in.data = buf;
    msg_in.length = MSG_T_MAX;
    uint32_t result = 1;
    while (result) {
      result = recv(&msg_in, p);
      if (result == 0) {
        msg_out.data = buf;
        msg_out.length = strlen(buf);
        send(&msg_out, msg_in.sender);
      }
    }
  }
}
