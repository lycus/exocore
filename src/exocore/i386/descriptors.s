section .text

global i386_gdt_flush

align 4
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

align 4
.flush:

    ret

global i386_idt_flush

align 4
i386_idt_flush:

    ; IDT pointer is the first argument.
    mov eax, [esp + 4]

    ; Just in case...
    cli

    ; Load the IDT.
    lidt [eax]

    ; Enable interrupts.
    sti

    ret
