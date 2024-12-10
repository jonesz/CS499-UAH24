#include "program/program.h"
#include "syscalls/syscalls.h"
#include <stdint.h>
#include <stddef.h>
#include "libc/string.h"

static uint32_t read(char *buf, size_t len) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = len;
  uint32_t result = 1;
  while (result) {
    result = recv(&msg, 2);
  }

  return msg.length;
}

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

void dumb_fs() {
  char buf[MSG_T_MAX] = {0};

  while (1) {
    printf("Started filesystem...\n");
    if (read(buf, MSG_T_MAX)) {
      printf("YEALLO\n");
    }
  }
}
