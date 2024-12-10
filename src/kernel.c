/** src/kernel.c */
#include "sched/sched.h"
#include "mem/kmalloc.h"
#include "mem/fixed_alloc.h"
#include "mem/bump_alloc.h"
#include "mem/multiboot.h"
#include "vid/term.h"
#include "interrupt/acpi.h"
#include "interrupt/asm_tools.h"
#include "interrupt/interrupt.h"
#include "test/test.h"
#include <stddef.h>
#include "libc/string.h"
#include "syscalls/syscalls.h"
#include "program/program.h"
#include "ipc/ipc.h"

extern multiboot_info_t *boot_info;
static void spin() {
  while (1) {
    asm("nop");
  }
}

void kernel_main() {
  term_init();
  term_write("MiniOS Kernel Team #1\n");
  term_writeline("Testing writeline");
  term_err("This is an error\n");
  term_warn("This is a warning\n");

  term_format("This is a format string: %s.\n", "Hello");
  unsigned int tmp = 0xFFFFAAAA;
  void *loc = kernel_main;
  term_format("This is a format string hex: %x\n", &tmp);
  term_format("This is a format string hex: %x\n", &loc);
  term_write((const char *)boot_info->cmdline);
  term_write("\n");

  // This ridiculous code finds the RSDP, which is used to find the ACPI
  // The ACPI holds important system information
  uint8_t sum = 0;
  uint32_t sum32 = sum;
  RSDP_t *rsdp = 0;
  char *rsd_ptr_str = "RSD PTR ";
  for (uint8_t *i = (uint8_t *)0x000E0000; i < (uint8_t *)0x000FFFFF; i++) {
    if (strncmp(rsd_ptr_str, (char *)i, 8) == 0) {
      rsdp = (RSDP_t *)i;
    }
  }
  // Checksum to make sure RSDP is valid
  for (size_t i = 0; i < sizeof(*rsdp); i++) {
    sum += *(uint8_t *)((uint8_t *)rsdp + i);
  }
  sum32 = sum;
  if (sum != 0) {
    term_format("Error: RSDP Checksum Failed - %x\n", &sum32);
    return;
  }
  sum = 0;

  // After verifying the RSDP, move on to the actual Root System Description
  RSDT *rsdt = (RSDT *)rsdp->RsdtAddress;
  for (size_t i = 0; i < rsdt->h.Length; i++) {
    sum += *(uint8_t *)((uint8_t *)rsdt + i);
  }
  sum32 = sum;
  if (sum != 0) {
    term_format("Error: RSDT Checksum Failed - %x\n", &sum32);
    return;
  }
  sum = 0;

  // After verifying the RSDT, move on to the actual FADT to check for a serial
  // controller
  FADT *fadt = (FADT *)rsdt->PointerToOtherSDT;
  // Signature 'FADT' (non-null-termianted, ignore trailing characters) shows if
  // we are At the correct table
  term_format("%s\n", fadt->h.Signature);
  for (size_t i = 0; i < fadt->h.Length; i++) {
    sum += *(uint8_t *)((uint8_t *)fadt + i);
  }
  sum32 = sum;
  if (sum != 0) {
    term_format("Error: FADT Checksum Failed - %x\n", &sum32);
    return;
  }

  /* NOTE(BP): BootArchitectureFlags should tell us if a serial controller
  exists As a double check, The value should be at offset 109 according to this
  page
  https://wiki.osdev.org/%228042%22_PS/2_Controller#Step_2:_Determine_if_the_PS/2_Controller_Exists
  // These are off by what appears to be one byte, but the relevant flag is zero
  either way on my machine. So, it seems that there is no serial controller for
  I/O
  */
  term_format("BAF: %x\n", &(fadt->BootArchitectureFlags));
  term_format("BAF (DOUBLE CHECK): %x\n", (uint8_t *)(fadt) + 109);

  for (size_t i = 0; i < (rsdt->h.Length - sizeof(rsdt->h)) / 4; i++) {
    term_format("Table: %s\n", *((uint32_t **)&(rsdt->PointerToOtherSDT) + i));
  }

  ipc_init();

  // TODO: What should these be?
  fixed_alloc_init(0x4000000, 4096 * 512, 4096);
  bump_alloc_init( 0x4000000 + (4096 * 512) + 1, 4096 * 10);

  sched_admit((uint32_t)shell_main);
  // Include a do-nothing process so if the shell blocks, we can still jump to a process that does something.
  sched_admit((uint32_t)spin);
  sched_admit((uint32_t)dumb_fs);

  init_pic();

  while (1) {
    asm("nop");
  }
}

// volatile void process_1() {
//   volatile uint32_t idx = 0;
//   uint32_t overflows = 0;
// 
//   char buf[MSG_T_MAX] = {0};
//   uint32_t buflen = MSG_T_MAX;
//   msg_t msg = {0};
//   msg.data = buf;
//   msg.length = buflen;
// 
//   while (1) {
//     if (idx == 0) {
//       //term_format("process 1: overflowed %x times\n", &overflows);
//   
//       overflows += 1;
//     }
//     // TODO(Britton): An unknown bug causes this to result in an "Invalid Opcode" on some builds,
//     // Diagnose and fix
//     if(recv(&msg, 0)) {
//       term_format("RECV: %s", msg.data);
//     }
//     idx = (idx + 1) & 0xFFFFFF;
//   }
// }
// 
// volatile void process_2() {
//   volatile uint32_t idx = 0;
//   uint32_t overflows = 0;
//   while (1) {
//     if (idx == 0) {
//       //term_format("process 2: overflowed %x times\n", &overflows);
//       overflows += 1;
//     }
// 
//     idx = (idx + 1) & 0xFFFFFF;
//   }
// }
