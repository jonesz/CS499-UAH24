#include "syscalls/syscalls.h"
#include "syscalls/syscalls_internal.h"
#include "sched/sched.h"
#include <stdint.h>
#include "vid/term.h"
#include "ipc/ipc.h"

extern ringbuffer_t process_buffers[MAX_PROCESSES];
extern ringbuffer_t ipc_stdin;

uint32_t send(msg_t* msg, uint32_t comm_channel) {
    send_args_t args = {0};
    syscall_info_t syscall_info = {0};
    args.msg = msg;
    args.comm_channel = comm_channel;
    syscall_info.args = &args;
    syscall_info.id = Sys_Send;
    return swint(&syscall_info);
}

uint32_t recv(msg_t* msg_dest, uint32_t comm_channel) {
    recv_args_t args = {0};
    syscall_info_t syscall_info = {0};
    args.msg_dest = msg_dest;
    args.comm_channel = comm_channel;
    syscall_info.args = &args;
    syscall_info.id = Sys_Recv;
    return swint(&syscall_info);
}

uint32_t sleep(uint32_t ticks) {
    sleep_args_t args = {0};
    args.ticks = ticks;
    syscall_info_t syscall_info = {0};
    syscall_info.args = &args;
    syscall_info.id = Sys_Sleep;
    return swint(&syscall_info);
}

uint32_t exit() {
    syscall_info_t syscall_info = {0};
    syscall_info.id = Sys_Exit;
    return swint(&syscall_info);
}

// Handle the syscall; this is called by the interrupt handler. In a proper world, the above runs
// in userspace and the below runs in kernel space.
void handle_syscall(uint32_t stack_loc) {
    syscall_info_t info =
        *(syscall_info_t *)(*(uint32_t *)(stack_loc - (4 * 0)));
    uint32_t *eax = (uint32_t *)(stack_loc - (4 * 0));

    switch (info.id)
    {
    case Sys_Send:
    {
        send_args_t* args = info.args;
        uint32_t length = args->msg->length;
        char* src = args->msg->data;

        // If the comm_channel is STDOUT, go ahead and write it to the screen.
        if (args->comm_channel = STDOUT) {
            for (int i = 0; i < length && i < MSG_T_MAX; i++) {
                term_write_char(&src[i]);
            }
            *eax = 0;
        } else { 
            // TODO: This might not be needed if we go ahead and just write to STDIN from the keyboard driver.
            if (args->comm_channel == STDIN) {
                *eax = ringbuffer_write_bytes(&ipc_stdin, src, length);
            } else if (args->comm_channel > MAX_PROCESSES) {
                *eax = 1; 
                return;
            }
            *eax = ringbuffer_write_bytes(&process_buffers[args->comm_channel], src, length);
        }
    }
        break;

    case Sys_Recv:
    {
        recv_args_t* args = info.args;
        // if (args->comm_channel == STDIN) {
        //     // *eax = ringbuffer_write_bytes(&ipc_stdin, )
        // }

        // if (ringbuffer_read()) {
        //     char* dest = args->msg_dest->data;
        //     for (uint32_t i = 0; i < msg_length && i < MSG_T_MAX; i++) {
        //         dest[i] = msg_buf[i];
        //     }
        //     args->msg_dest->length = msg_length;
        // } else {
        //     // There's no message able to be received, block the process.
        //    sched_block(stack_loc);
        // }
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
