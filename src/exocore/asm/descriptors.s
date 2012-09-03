%include "exocore/asm/segments.s"

section .text

global gdt_flush

align 8
gdt_flush:

    ; Load the GDT.
    lgdt [rdi]

    ret

global idt_flush

align 8
idt_flush:

    ; Load the IDT.
    lidt [rdi]

    ret
