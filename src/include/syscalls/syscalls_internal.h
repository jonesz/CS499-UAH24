#ifndef SYS_INTERNAL_H
#define SYS_INTERNAL_H

#include "syscalls.h"

typedef struct _send_args_t send_args_t;
typedef struct _recv_args_t recv_args_t;
typedef struct _sleep_args_t sleep_args_t;

struct _send_args_t {
    msg_t* msg;
    uint32_t comm_channel;
};
struct _recv_args_t {
    uint32_t comm_channel;
};
struct _sleep_args_t {
    uint32_t ticks;
};

void swint(syscall_info_t* syscall_info);
void handle_syscall(syscall_info_t info);

#endif
