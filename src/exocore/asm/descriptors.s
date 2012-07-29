%include "exocore/asm/segments.s"

section .text

global gdt_flush

align 8
gdt_flush:

%ifdef EXOCORE_IS_32_BIT
    ; GDT pointer is the first argument.
    mov eax, [esp + 4]

    ; Load the GDT.
    lgdt [eax]

    ; Load segment selectors.
    mov ax, KERNEL_DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to new code segment.
    jmp KERNEL_CODE_SEGMENT:.flush

align 8
.flush:
%else

    ; Load the GDT.
    lgdt [rdi]
%endif

    ret

global idt_flush

align 8
idt_flush:

%ifdef EXOCORE_IS_32_BIT
    ; IDT pointer is the first argument.
    mov eax, [esp + 4]

    ; Load the IDT.
    lidt [eax]
%else
    ; Load the GDT.
    lidt [rdi]
%endif

    ret
