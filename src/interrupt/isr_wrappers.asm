; Rather than rewrite a bunch of ISRs in assembly, push an OpCode to the handler 
; telling it which interrupt we've encountered.

%macro isr 1
global isr%1
isr%1:
        cli

        pushad
        mov eax, [esp+12] ; pushad pushes EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI. So the original ESP exists at [esp+12]?
        sub eax, 4        ; point the stack loc at EAX rather than the mem before it.
        push eax
        mov eax, %1
        push eax
        cld
        extern interrupt_handler
        call interrupt_handler
        pop eax
        pop eax
        popad
        sti
        iret
%endmacro

%macro isr_error_code 1
global isr%1:
isr%1:
        cli

        pushad
        mov eax, [esp+12] ; pushad pushes EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI. So the original ESP exists at [esp+12]?
        sub eax, 4        ; point the stack loc at EAX rather than the mem before it.
        push eax
        mov eax, %1
        push eax
        cld
        extern interrupt_handler
        call interrupt_handler
        pop eax
        pop eax
        popad
        add esp, 4        ; The fault handler pushed a return code, we need to take care of it here.
        sti
        iret
%endmacro

%assign i 0
%rep 256
        isr i
%assign i i+1
%endrep
