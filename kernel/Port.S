.section .text
.intel_syntax noprefix

.global write_port
.global read_port
.global load_idt
.global debug

.global keyboard_handler

read_port:
    mov edx, [esp + 4]
    in al, dx
    ret

write_port:
    mov edx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

keyboard_handler:
    mov al, 0x20
    out 0x20, al

    call keyboard_handler_main
    iretd
