#include "syscalls/syscalls.h"
#include "syscalls/syscalls_internal.h"
#include "sched/sched.h"
#include <stdint.h>
#include "vid/term.h"

static char msg_buf[MSG_T_MAX];
static uint8_t msg_present;
static uint32_t msg_length;

uint32_t send(msg_t* msg, uint32_t comm_channel) {
    if (!msg_present) {
        send_args_t args = {0};
        syscall_info_t syscall_info = {0};
        args.msg = msg;
        args.comm_channel = comm_channel;
        syscall_info.args = &args;
        syscall_info.id = Sys_Send;
        swint(&syscall_info);
        return 1;
    }
    else {
        return 0;
    }
}
uint32_t recv(msg_t* msg_dest, uint32_t comm_channel) {
    if (msg_present) {
        recv_args_t args = {0};
        syscall_info_t syscall_info = {0};
        args.msg_dest = msg_dest;
        args.comm_channel = comm_channel;
        syscall_info.args = &args;
        syscall_info.id = Sys_Recv;
        swint(&syscall_info);
        return 1;
    }
    else {
        return 0;
    }
    return 0;
}

uint32_t sleep(uint32_t ticks) {
    sleep_args_t args = {0};
    args.ticks = ticks;
    syscall_info_t syscall_info = {0};
    syscall_info.args = &args;
    syscall_info.id = Sys_Sleep;
    swint(&syscall_info);
    return 1;
}

uint32_t exit() {
    syscall_info_t syscall_info = {0};
    syscall_info.id = Sys_Exit;
    swint(&syscall_info);
    return 1;
}

uint32_t write(msg_t* msg) {
    // TODO: Implement a "write" syscall.
    return 1;
}

void handle_syscall(uint32_t stack_loc) {
    syscall_info_t info =
        *(syscall_info_t *)(*(uint32_t *)(stack_loc - (4 * 0)));

    switch (info.id)
    {
    case Sys_Send:
    {
        send_args_t* args = info.args;
        uint32_t length = args->msg->length;
        char* src = args->msg->data;
        if (!msg_present) {
            for (uint32_t i = 0; i < length && i < MSG_T_MAX; i++) {
                msg_buf[i] = src[i];
            }
            msg_length = length;
            msg_present = 1;
            // If the comm channel is to STDOUT, write it to STDOUT.
            if (args->comm_channel == STDOUT) {
                term_format("%s", msg_buf);
            }
        }
    }
        break;

    case Sys_Recv:
    {
        recv_args_t* args = info.args;
        if (msg_present) {
            char* dest = args->msg_dest->data;
            for (uint32_t i = 0; i < msg_length && i < MSG_T_MAX; i++) {
                dest[i] = msg_buf[i];
                msg_buf[i] = 0;
            }
            args->msg_dest->length = msg_length;
            msg_length = 0;
            msg_present = 0;
        }
    }
    break;

    case Sys_Sleep: 
    {
        // TODO(Britton): This should somehow tell the scheduler to block 
        // For "ticks" ticks
        sleep_args_t* args = info.args;
        uint32_t ticks = args->ticks;
        term_err("Sleep: ");
        term_format("%x\n", &ticks);
    }
    break;

    case Sys_Exit:
        break;

    default:
    term_write("Unk: ");
        break;
    }
}
