; boot.asm, NASM. Ripped from https://wiki.osdev.org/Bare_Bones_with_NASM
; Multiboot specification: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
MBMAGIC equ 0x1BADB002 ; identifier for the multiboot header.
MBALIGN equ 1 << 0     ; require that all boot modules loaded are page boundary aligned.
MBMEM   equ 1 << 1     ; 'mem_*' fields must be included.
MBFLAGS equ MBALIGN | MBMEM
MBCHECKSUM equ -(MBMAGIC + MBFLAGS) ; this when added to the other fields equals zero.

; Declare the multiboot header as according to the multiboot standard, linked above.
section .multiboot
align 4
        dd MBMAGIC
        dd MBFLAGS
        dd MBCHECKSUM

; Set up a stack. An x86 stack must be 16-byte aligned according to the System V ABI standard.
section .bss
align 16
stack_bottom:
        resb 16384 ; 16 KiB reserved.
stack_top:

; Prepare space for the GDT
section .data
align 32
gdt_base times 48 db 0 ; 8 byte long GDT entries * 6 required entries = 48B reserved
align 16
gdt_descriptor  DW 0
                DD 0

; Bootloader jumps to this `_start` as specified by the linker.
section .text

global _start:function (_start.end - _start)
_start:

; We're within a 32-bit protected mode on x86. Zero interrupts,
; paging is disabled. Setup the stack.
        mov esp, stack_top

; TODO: Setup Paging here and any state that is needed before
; the kernel is loaded.
        ; Sets up gdt
        mov eax, gdt_descriptor
        mov ebx, gdt_base
        mov word [gdt_descriptor], 47
        mov [gdt_descriptor + 2], ebx 
        push ebx
        extern set_up_gdt
        call set_up_gdt
        lgdt [gdt_descriptor]
        ; TODO(Britton): Trying to reload cs fails on my machine,
        ; but in testing it was already set to 8.
        ; Figure out why it is not working to be sure we manually set it.
        ; jmp 0x8:reload_cs
        .reload_cs:
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

; Enter the main kernel.
        extern kernel_main
        call kernel_main

; If the above call exits, disable interrupts and then enter an
; infinite loop.
        cli
.hang: hlt
        jmp .hang
.end:
