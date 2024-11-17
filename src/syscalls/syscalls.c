#include "syscalls/syscalls.h"
#include "syscalls/syscalls_internal.h"
#include <stdint.h>
#include "vid/term.h"

uint32_t send(msg_t* msg, uint32_t comm_channel) {
    send_args_t args = {0};
    syscall_info_t syscall_info = {0};
    args.msg = msg;
    args.comm_channel = comm_channel;
    syscall_info.args = &args;
    syscall_info.id = Sys_Send;
    swint(&syscall_info);
    return 1;
}
msg_t* recv(uint32_t comm_channel) {
    return 0;
}
uint32_t sleep(uint32_t ticks) {
    return 0;
}

void handle_syscall(syscall_info_t info) {
    switch (info.id)
    {
    case Sys_Send:
        send_args_t* args = info.args;
        uint32_t x = args->comm_channel;
        term_format("SYS_SEND: %x\n", &x);
        break;
    
    default:
    term_write("Unk\n");
        break;
    }
}