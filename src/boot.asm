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

; Prepare space for the Page Directory
align 4096
page_directory times 1024 DD 0

align 4096
page_tables times 1024 * 1024 DD 0

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
        ; Disable interrupts while setting up the GDT and paging
        cli
        ; First, set up the GDT descriptor,
        ; which the CPU will use to find and interpret the GDT
        mov eax, gdt_descriptor
        mov ebx, gdt_base
        ; The bottom 2 bytes of the GDT descriptor are the size in bytes of the GDT minus 1
        mov word [gdt_descriptor], 47
        ; The top 4 bytes are the address of the GDT
        mov [gdt_descriptor + 2], ebx 
        ; Push the address of the GDT to the stack for function call
        push ebx
        extern set_up_gdt
        call set_up_gdt
        ; Once GDT is set up, lgdt with the GDT descriptor the set it
        lgdt [gdt_descriptor]
        jmp 0x8:_start.reload_cs
        .reload_cs:
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        ; Set up identity Page tables
        ; TODO(Britton): Support non-identity paging
        mov eax, page_tables
        push eax
        mov eax, page_directory
        push eax
        extern page_map_identity
        call page_map_identity
        ; Enable Paging
        mov eax, page_directory
        mov cr3, eax
        mov eax, cr0
        or eax, 0x80000001
        mov cr0, eax
        mov eax, cr0
        ; re-enable interrupts
        sti
; Enter the main kernel.
        extern kernel_main
        call kernel_main

; If the above call exits, disable interrupts and then enter an
; infinite loop.
        cli
.hang: hlt
        jmp .hang
.end:
