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

__attribute__((cdecl)) void pong_main(int argc, char **argv) {	
	uint32_t p = pid();
	printf("Starting pong server with PID: ");
	// TODO: Change this to a printf.
	term_format("%x\n", &p);

	char buf[MSG_T_MAX];
	msg_t msg_in = {0};
	msg_in.data = buf;
	msg_in.length = MSG_T_MAX;

	while(1) { 
		uint32_t result = recv(&msg_in, p);
		if (result == 0) {
			printf("PONG!\n");
		} 
	}

	return;
}
