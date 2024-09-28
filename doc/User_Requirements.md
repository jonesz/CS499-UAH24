# Usage Requirements

## Concurrency
The user will be able to multitask by running multiple programs at once.

## Input/Output
The user will interact with the environment using the keyboard and the system will respond.

## Data
The user will be able to save and read files from permanent storage, and they will be able to navigate to desired storage.

## Security
The system should have basic security measures, including simple memory protection.

# Components

## Virtual Memory
Virtual memory will allow both *Security* and *Concurrency* by giving the kernel the tools necessary to manage RAM.

## Interrupts
Interrupts will allow *Input/Output* by allowing the kernel to detect interrupt driven I/O such as the keyboard.
It will also allow *Concurrency* by using timer interrupts to switch processes.

## Processes/Scheduling
Processes/Scheduling will allow *Concurrency* by defining a system the kernel uses to switch between different tasks that the user has request it to complete.

## Filesystem
The filesystem implementation will allow management of *Data* by giving the kernel control of permanent storage devices, including systems necessary for reading and writing to files, and navigating directories.

