#include "libc/string.h"
#include "program/program.h"
#include "syscalls/syscalls.h"
#include "vid/term.h"

static uint32_t str_toint(char *s) {
  uint32_t parsing = 1;
  uint32_t i = 0;
  uint32_t sum = 0;

  while (parsing) {

    uint8_t x = s[i];
    x -= '0';
    if (x > 9) {
      parsing = 0;
    } else {
      sum *= 10;
      sum += x;
    }
    i++;
  }
}

static void printf(char *buf) {
  msg_t msg = {0};
  msg.data = buf;
  msg.length = strlen(buf);
  send(&msg, STDOUT);
}

__attribute__((cdecl)) void ping_main(int argc, char **argv) {	
	uint32_t p = pid();
	msg_t msg_out = {0};
	msg_out.data = "PING";
	msg_out.length = strlen("PING");
	msg_out.sender = pid();

	printf("Sending PING to "); 
	printf(argv[1]);

	uint32_t i = str_toint(argv[1]);
	term_format("i: %x\n", &i);
	send(&msg_out, 3);

	char buf[MSG_T_MAX];
	msg_t msg_in = {0};
	msg_in.data = buf;
	msg_in.length = MSG_T_MAX;

	/*
	while(1) { 
		uint32_t result = recv(&msg_in, p);
		if (result == 0) {
			printf(msg_in.data);
			return;
		}
	}
*/	

	return;
}
