#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define MSG_T_MAX 1024

typedef enum _syscall_id_t syscall_id_t;
typedef struct _syscall_info_t syscall_info_t;

typedef struct _msg_t msg_t;

enum _syscall_id_t {Sys_Send, Sys_Recv, Sys_Sleep, Sys_Exit, Sys_Write};

struct  _syscall_info_t {
    void* args;
    syscall_id_t id;
};

struct  _msg_t {
    void* data;
    uint32_t length;
};


uint32_t send(msg_t* msg, uint32_t comm_channel);
uint32_t recv(msg_t* msg_dest, uint32_t comm_channel);
uint32_t sleep(uint32_t ticks);
uint32_t exit();
uint32_t write(msg_t* msg);

#endif
