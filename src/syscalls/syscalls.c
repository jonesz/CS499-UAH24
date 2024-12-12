#include "syscalls/syscalls.h"
#include "ipc/ipc.h"
#include "sched/sched.h"
#include "syscalls/syscalls_internal.h"
#include "vid/term.h"
#include <stdint.h>

extern ringbuffer_t process_buffers[MAX_PROCESSES];
extern ringbuffer_t ipc_stdin;
extern uint32_t writer[MAX_PROCESSES];

uint32_t send(msg_t *msg, uint32_t comm_channel) {
  send_args_t args = {0};
  syscall_info_t syscall_info = {0};
  args.msg = msg;
  args.comm_channel = comm_channel;
  syscall_info.args = &args;
  syscall_info.id = Sys_Send;
  return swint(&syscall_info);
}

uint32_t recv(msg_t *msg_dest, uint32_t comm_channel) {
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

uint32_t pid() {
  syscall_info_t syscall_info = {};
  syscall_info.id = Sys_PID;
  return swint(&syscall_info);
}

// TODO(BP): Implement argv because it is currently unused
uint32_t spawn(uint32_t eip, uint32_t argc, char **argv) {
  spawn_args_t args = {0};
  args.eip = eip;
  args.argc = argc;
  args.argv = argv;
  syscall_info_t syscall_info = {0};
  syscall_info.args = &args;
  syscall_info.id = Sys_Spawn;
  return swint(&syscall_info);
}

uint32_t spawn_bg(uint32_t eip, uint32_t argc, char **argv) {
  spawn_args_t args = {0};
  args.eip = eip;
  args.argc = argc;
  args.argv = argv;
  syscall_info_t syscall_info = {0};
  syscall_info.args = &args;
  syscall_info.id = Sys_Spawn_Bg;
  return swint(&syscall_info);
}

// Handle the syscall; this is called by the interrupt handler. In a proper
// world, the above runs in userspace and the below runs in kernel space.
void handle_syscall(uint32_t stack_loc) {
  syscall_info_t info = *(syscall_info_t *)(*(uint32_t *)(stack_loc - (4 * 0)));
  uint32_t *eax = (uint32_t *)(stack_loc - (4 * 0));
  uint32_t pid;

  switch (info.id) {
  case Sys_Send: {
    send_args_t *args = info.args;
    uint32_t length = args->msg->length;
    char *src = args->msg->data;

    // If the comm_channel is STDOUT, go ahead and write it to the screen.
    if (args->comm_channel == STDOUT) {
      for (int i = 0; i < length && i < MSG_T_MAX; i++) {
        term_write_char(&src[i]);
      }
      *eax = 0;
      return;
    } else {
      // TODO: This might not be needed if we go ahead and just write to STDIN
      // from the keyboard driver.
      if (args->comm_channel == STDIN) {
        *eax = ringbuffer_write_bytes(&ipc_stdin, src, length);
        // We wrote to STDIN, we'll go ahead and unblock the shell.
        sched_unblock(STDIN);
        *eax = 0;
        return;
      } else if (args->comm_channel > MAX_PROCESSES) {
        *eax = 1;
        return;
      } else {
        *eax = ringbuffer_write_bytes(&process_buffers[args->comm_channel], src,
                                    length);
        writer[args->comm_channel] = args->msg->sender;
        sched_unblock(args->comm_channel);
        return;
      }
    }
  } break;

  case Sys_Recv: {
    recv_args_t *args = info.args;

    // Attempt to read from STDIN.
    if (args->comm_channel == STDIN) {
      uint8_t *dst = args->msg_dest->data;
      // Check if there's a message within STDIN to read, if there's not, block.
      if (ringbuffer_read(&ipc_stdin, dst)) {
        // TODO: There was no message, block the currently running process and
        // return 1.
        sched_block(stack_loc, STDIN);
        *eax = 1;
        return;
      } else {
        int i = 1; // We've read a single bit.
        while (!ringbuffer_read(&ipc_stdin, (dst + i)) && i < MSG_T_MAX) {
          i++;
        }
        args->msg_dest->length = i;
        *eax = 0;
        return;
      }
    } else {
      uint8_t *dst = args->msg_dest->data;
      args->msg_dest->sender = writer[args->comm_channel];
      args->msg_dest->length = 0;
      if (ringbuffer_read(&process_buffers[args->comm_channel], dst)) {
        sched_block(stack_loc, args->comm_channel);
        *eax = 1;
        return;
      } else {
        int i = 1;
        while (!ringbuffer_read(&process_buffers[args->comm_channel], (dst + i)) && i < MSG_T_MAX) {
          i++;
        }
        args->msg_dest->length = i;
        *eax = 0;
        return;
      }
    }
  } break;

  case Sys_Sleep: {
    // TODO(Britton): This should somehow tell the scheduler to block
    // For "ticks" ticks
    sleep_args_t *args = info.args;
    uint32_t ticks = args->ticks;
    term_err("Sleep: ");
    term_format("%x\n", &ticks);
  } break;

  // TODO(BP): Sys_Exit and Sys_Spawn should know why they are blocking and
  // unblocking, so that processes can be unblocked for the correct reason
  case Sys_Exit:
    pid = sched_kill(stack_loc);
    sched_unblock(pid);
    break;

  case Sys_Spawn: {
    spawn_args_t *args = info.args;
    pid = sched_admit_args(args->eip, args->argc, args->argv);
    sched_block(stack_loc, pid);
  } break;
  
  case Sys_Spawn_Bg: {
    spawn_args_t *args = info.args;
    sched_admit_args(args->eip, args->argc, args->argv);
  } break;

  case Sys_PID:
    pid = sched_running();
    *eax = pid;
    break;

  default:
    term_format("swint: %x", &(info.id));
    break;
  }
}
