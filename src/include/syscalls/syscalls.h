#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

typedef enum _syscall_id_t syscall_id_t;
typedef struct _syscall_info_t syscall_info_t;

typedef struct _msg_t msg_t;

enum _syscall_id_t {Sys_Send, Sys_Recv, Sys_Sleep};

struct  _syscall_info_t {
    void* args;
    syscall_id_t id;
};

struct  _msg_t {
    uint32_t tmp;
};


uint32_t send(msg_t* msg, uint32_t comm_channel);
msg_t* recv(uint32_t comm_channel);
uint32_t sleep(uint32_t ticks);

#endif
