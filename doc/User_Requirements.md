# Project Requirements

Our group will deliver an i386 OS capable of running on QEMU; this can further be seperated into three
main deliverables: the project source code, the kernel documentation, and a prebuilt .iso file.

Below are the specific, user facing features we believe are important for this product.

# Features

## Concurrency
The User will be able to multitask by running multiple programs at once. As an example, a User could run a program that computes a 
mathematical formula while also running a program that reads from a file. This *Concurrency* should be seamless and not require major 
User intervention.

## Input/Output
The User will interact with the environment using the keyboard and the system will respond. As an example, a User could write out the phrase `echo "Hello World"`
and have a userspace `echo` program with the input `Hello World`.

## Data
The User will be able to save and read files from permanent storage and they will be able to navigate to desired storage. As an example, the User could write the output
of a program to a file.

## Security
The system should have basic security measures, including simple memory protection. As an example, a process started by a User should not be able to modify kernel memory.

# Components

We will accomplish these *Features* by building the following top-level OS *Components*.

## Virtual Memory
Virtual memory will allow both *Security* and *Concurrency* by giving the kernel the tools necessary to manage memory.

## Interrupts
Interrupts will allow *Input/Output* by allowing the kernel to detect interrupt driven I/O such as the keyboard.
It will also allow *Concurrency* by using timer interrupts to switch processes.

## Processes/Scheduling
Processes/Scheduling will allow *Concurrency* by defining a system the kernel uses to switch between different tasks that the User has started.

## Filesystem
The filesystem implementation will allow management of *Data* by giving the kernel control of permanent storage devices, including systems necessary for reading and writing to files.

## System Calls, Userspace Utilities
The system calls and userspace utilities will allow for the User to function in userspace, accomplishing the features of *Input/Output* and *Data*.
