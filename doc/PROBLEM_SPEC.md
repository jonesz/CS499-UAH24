*Problem Spec; retrieved 8/22/2024.*

# "MiniOS: A Custom Lightweight Operating System Kernel"

## Overview: 
MiniOS is a simple, lightweight operating system kernel designed to demonstrate core OS concepts such as process scheduling, memory management, file handling, and device drivers. This project involves building a basic OS from the ground up, giving students hands-on experience with systems programming, low-level hardware interaction, and resource management.
Key Features

## 1. Kernel Initialization and Bootloader
Custom Bootloader: Develop a bootloader (e.g., using GRUB or a custom bootloader) that initializes the hardware and loads the kernel into memory.
Kernel Initialization: Write the initialization routines to set up the kernel, including setting up memory, initializing device drivers, and starting the first process.
Interrupt Handling: Implement basic interrupt handling to manage hardware interrupts like keyboard input, timers, and system calls.

## 2. Process Scheduling
Process Control Blocks (PCBs): Implement data structures to represent processes, including state, program counter, stack pointer, and other metadata.
Basic Process Scheduling (Round Robin or Priority Scheduling): Implement a simple scheduling algorithm to switch between processes. For example, round-robin scheduling gives each process a fixed time slice, while priority scheduling assigns priorities to processes.
Context Switching: Implement context switching to allow the OS to switch between processes by saving and restoring CPU registers and process states.
Multitasking Support: Allow for multiple processes to run concurrently, giving the illusion of parallelism on a single CPU core.

## 3. Memory Management
Physical Memory Management: Implement a memory manager that tracks available memory and allocates/free memory as needed by processes.
Virtual Memory (Optional): Implement a simple virtual memory system using paging to provide each process with its own address space, preventing processes from accessing each other’s memory.
Memory Allocation Algorithms: Use basic memory allocation algorithms such as first-fit, best-fit, or buddy system to manage memory efficiently.
Page Fault Handling (Optional): Implement page fault handling to swap pages in and out of memory when needed.

## 4. File System
Basic File System Support: Implement a simple file system that allows users to create, read, write, and delete files. This could be based on FAT (File Allocation Table) or a custom file system.
Directory Management: Implement directory structures to allow hierarchical file management (e.g., directories and subdirectories).
File Permissions (Optional): Implement basic file permissions for user-level access control (e.g., read/write/execute permissions).

## 5. System Calls
System Call Interface: Implement a set of system calls that allow user programs to interact with the OS kernel. These could include calls for process management (e.g., fork, exec, wait), file I/O (open, read, write), and memory allocation (malloc, free).
User and Kernel Mode Switching: Implement protection rings or privilege levels, ensuring that user programs run in user mode and system calls switch to kernel mode for safe execution.
Inter-Process Communication (IPC): Implement basic IPC mechanisms like pipes, message queues, or shared memory, allowing processes to communicate and synchronize with each other.

## 6. Basic Device Drivers
Keyboard Driver: Implement a basic driver to handle keyboard input, allowing the OS to receive and process user input.
Display Driver: Implement a basic text-mode display driver (e.g., using VGA) to allow the OS to output text to the screen.
Timer Driver: Implement a driver to manage the system timer, which can be used for process scheduling (e.g., generating interrupts at regular intervals to trigger context switches).

## 7. Shell Interface
Command-Line Shell: Develop a simple shell interface that allows users to interact with the OS through commands. The shell can support basic commands such as listing files, launching programs, and managing processes.
Built-In Commands: Implement commands such as ls (list files), cd (change directory), cp (copy files), rm (remove files), and ps (list running processes).
Background and Foreground Processes: Allow users to run processes in the background (using &) or foreground and handle process termination and suspension.

## 8. Basic Networking (Optional)
Network Stack: Implement basic networking support, such as sending/receiving data over TCP/IP. This could involve a simple socket API that allows processes to open sockets and communicate over the network.
Packet Handling: Implement packet handling for basic networking protocols like IP, TCP, and UDP.

## 9. Testing and Debugging Tools
Kernel Logging: Implement logging functionality within the kernel to track system events such as process creation, memory allocation, and device interactions.
Kernel Panic and Debugging: Implement error handling and a simple kernel panic mechanism that outputs diagnostic information when a critical error occurs.
User-Level Debugger (Optional): Develop a simple user-level debugger that allows developers to set breakpoints, inspect memory, and step through processes.

## 10. Multithreading Support (Optional)
Thread Management: Implement basic threading functionality, allowing multiple threads to run within a single process. This would involve creating thread control blocks (TCBs) and handling context switches between threads.
Thread Synchronization: Provide mechanisms for synchronizing threads, such as mutexes, semaphores, or condition variables.

## Operating Systems Tools and Technologies to Use
### Low-Level Programming:
C/C++: For writing the OS kernel and system programs. C is commonly used for systems programming due to its low-level memory management and control over hardware.
Assembly Language (x86 or ARM): For writing bootloaders, interrupt handlers, and other low-level tasks that require direct interaction with hardware.
Makefiles: For building and compiling the OS.

### Virtualization and Emulation:
QEMU: A popular emulator to test the OS on a virtual machine. QEMU allows for debugging and testing without the need for physical hardware.
Bochs: Another emulator for testing x86-based OS kernels.
VirtualBox or VMware: For running the OS on virtual hardware.

### File Systems:
FAT12/FAT16/FAT32: A simple file system that can be implemented in a custom OS to manage files and directories.
Ext2 (Optional): A more advanced file system that could be implemented for Linux-based OS kernels.
Kernel Development:
GRUB (GRand Unified Bootloader): A bootloader commonly used for loading the OS kernel. GRUB supports multiboot-compliant kernels.
GCC (GNU Compiler Collection): For compiling C code into binaries that can run on the OS.
GDB (GNU Debugger): For debugging the OS and inspecting memory, registers, and processes.

### Development Environment:
Linux or Unix-Based System: For development and cross-compiling the OS kernel.
Cross-Compilers: Use cross-compilers to generate code for different CPU architectures (e.g., i386, x86_64, ARM).

## Feature Development Timeline (18 Weeks)

### Week 1-2: Bootloader and Kernel Initialization
Requirement 1: Write a basic bootloader that loads the kernel into memory.
Requirement 2: Implement kernel initialization, including setting up memory and device drivers.

### Week 3-4: Process Scheduling and Context Switching
Requirement 3: Develop the data structures for process control blocks (PCBs).
Requirement 4: Implement basic process scheduling (e.g., round-robin) and context switching.

### Week 5-6: Memory Management
Requirement 5: Implement physical memory management (memory allocation, freeing).
Requirement 6: Optionally implement paging and virtual memory management.

### Week 7-8: File System Implementation
Requirement 7: Implement a basic file system (e.g., FAT) to manage files and directories.
Requirement 8: Develop system calls for file manipulation (open, read, write, delete).

### Week 9-10: System Calls and User Programs
Requirement 9: Implement system calls for process management (fork, exec) and file I/O.
Requirement 10: Write simple user programs to test system calls.

### Week 11-12: Device Drivers and Interrupt Handling
Requirement 11: Develop basic device drivers for keyboard input and screen output.
Requirement 12: Implement interrupt handling for system timer and user input (e.g., keyboard interrupts).

### Week 13-14: Shell Interface
Requirement 13: Create a simple shell interface with basic commands (e.g., ls, cd, ps).
Requirement 14: Implement background and foreground process handling in the shell.

### Week 15-16: Multithreading and IPC (Optional)
Requirement 15: Implement basic multithreading support (thread creation, scheduling).
Requirement 16: Develop inter-process communication (pipes, message queues, or shared memory).

### Week 17: Testing and Debugging
Requirement 17: Test the OS on a virtual machine (QEMU, Bochs) and debug issues.
Requirement 18: Implement logging and debugging tools to assist with diagnosing kernel errors.

### Week 18: Final Testing and Documentation
Requirement 19: Perform final testing across different environments and complete documentation.
Requirement 20: Create a user manual and developer guide for the OS.

### Team Member Breakdown
Kernel Developer: Focus on kernel-level functionality, including process scheduling, memory management, and system calls.
File System Developer: Develop the file system, including file and directory management, and implement file I/O system calls.
Device Driver Developer: Focus on developing drivers for hardware devices (keyboard, display, timer).
Shell and User Interface Developer: Develop the shell interface and implement user-facing commands.
This project offers hands-on experience with core operating systems concepts, including process scheduling, memory management, file systems, and device drivers, providing a deep understanding of systems programming.
