#ifndef SYS_INTERNAL_H
#define SYS_INTERNAL_H

#include "syscalls.h"

typedef struct _send_args_t send_args_t;
typedef struct _recv_args_t recv_args_t;
typedef struct _sleep_args_t sleep_args_t;
typedef struct _spawn_args_t spawn_args_t;

struct _send_args_t {
    msg_t* msg;
    uint32_t comm_channel;
};
struct _recv_args_t {
    msg_t* msg_dest;
    uint32_t comm_channel;
};
struct _sleep_args_t {
    uint32_t ticks;
};

struct _spawn_args_t {
    uint32_t eip;
    uint32_t argc;
    char** argv;
    uint8_t argv_is_present;
};

uint32_t swint(syscall_info_t* syscall_info);
void handle_syscall(uint32_t stack_loc);

#endif
