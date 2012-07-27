section .text

global halt_machine

align 4
halt_machine:

    hlt
    jmp halt_machine
