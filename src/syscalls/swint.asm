global swint
swint:
    mov eax, [esp + 4]
    int 0x80
    ret
