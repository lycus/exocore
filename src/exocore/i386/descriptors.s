section .text

align 4

global i386_gdt_flush

i386_gdt_flush:

    ; GDT pointer is the first argument.
    mov eax, [esp + 4]

    ; Load the GDT.
    lgdt [eax]

    ; Load segment selectors.
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to new code segment.
    jmp 0x08:.flush

.flush:

    ret

global i386_idt_flush

i386_idt_flush:

    ; IDT pointer is the first argument.
    mov eax, [esp + 4]

    ; Just in case...
    cli

    ; Load the IDT.
    lidt [eax]

    ; TODO: Enable interrupts.
    ; sti

    ret
