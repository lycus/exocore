section .text

align 4

; Helper macro for defining ISRs with no error codes.
%macro isr_no_error 1
    global i386_isr_%1

    i386_isr_%1:

        ; Disable interrupts.
        cli

        push 0 ; Push a dummy error code.
        push %1 ; Push the interrupt identifier.

        ; Call the common stub that then calls the actual handler.
        jmp isr_common_stub
%endmacro

; Helper macro for defining ISRs with error codes.
%macro isr_error 1
    global i386_isr_%1

    i386_isr_%1:

        ; Disable interrupts.
        cli

        push %1 ; Push the interrupt identifier.

        ; Call the common stub that then calls the actual handler.
        jmp isr_common_stub
%endmacro

isr_no_error 0
isr_no_error 1
isr_no_error 2
isr_no_error 3
isr_no_error 4
isr_no_error 5
isr_no_error 6
isr_no_error 7
isr_error 8
isr_no_error 9
isr_error 10
isr_error 11
isr_error 12
isr_error 13
isr_error 14
isr_no_error 15
isr_no_error 16
isr_no_error 17
isr_no_error 18
isr_no_error 19
isr_no_error 20
isr_no_error 21
isr_no_error 22
isr_no_error 23
isr_no_error 24
isr_no_error 25
isr_no_error 26
isr_no_error 27
isr_no_error 28
isr_no_error 29
isr_no_error 30
isr_no_error 31

; C function for handling interrupts.
extern i386_isr_handler

isr_common_stub:

    ; Save all GPRs.
    pushad

    ; Back up the data segment descriptor.
    mov ax, ds
    push eax

    ; Load the kernel data segment descriptor.
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Expects a signature like: void i386_isr_handler(registers_t regs)
    call i386_isr_handler

    ; Reload the original data segment descriptor.
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad ; Pop all backed-up GPRs.
    add esp, 4 ; Pop the interrupt identifier and error code.

    ; Enable interrupts.
    sti

    ; Return from the ISR.
    iret
