#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include "sched/sched.h"

/* For information about this file, see the `MiniOS Programmer's Manual.` */

#define MSG_T_MAX 1024
#define STDOUT (MAX_PROCESSES + 1)
#define STDIN  (MAX_PROCESSES + 2)

typedef enum _syscall_id_t syscall_id_t;
typedef struct _syscall_info_t syscall_info_t;
typedef struct _msg_t msg_t;

enum _syscall_id_t {Sys_Send, Sys_Recv, Sys_Sleep, Sys_Exit, Sys_Spawn, Sys_Spawn_Bg, Sys_PID};

struct  _syscall_info_t {
    void* args;
    syscall_id_t id;
};

struct  _msg_t {
    void* data;
    uint32_t length;
    uint32_t sender;
};

/* For information about this file, see the `MiniOS Programmer's Manual.` */
uint32_t send(msg_t* msg, uint32_t comm_channel);
uint32_t recv(msg_t* msg_dest, uint32_t comm_channel);
// uint32_t sleep(uint32_t ticks);
uint32_t exit();
uint32_t spawn(uint32_t eip, uint32_t argc, char** argv);
uint32_t spawn_bg(uint32_t eip, uint32_t argc, char** argv);
uint32_t pid();

#endif
