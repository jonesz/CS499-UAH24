; Rather than rewrite a bunch of ISRs; push an OpCode to the handler telling it which interrupt we've
; encountered.

%macro isr 1
global isr_%1:
        cli
        pushad
        mov eax, %1
        push eax
        cld
        extern interrupt_handler
        call interrupt_handler
        pop eax
        popad
        sti
        iret
%endmacro

%assign i 0
%rep 256
        isr i
%assign i i+1
%endrep
