section .text

global enable_interrupts

align 4
enable_interrupts:

    sti
    ret

global disable_interrupts

align 4
disable_interrupts:

    cli
    ret

; Helper macro for defining ISRs with no error codes.
%macro isr_no_error 1
    global isr_%1

    align 4
    isr_%1:

        ; Disable interrupts.
        cli

        push 0 ; Push a dummy error code.
        push %1 ; Push the interrupt identifier.

        ; Call the common stub that then calls the actual handler.
        jmp isr_common_stub
%endmacro

; Helper macro for defining ISRs with error codes.
%macro isr_error 1
    global isr_%1

    align 4
    isr_%1:

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
extern isr_handler

align 4
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

    ; Expects a signature like: void isr_handler(interrupt_info_t)
    call isr_handler

    ; Reload the original data segment descriptor.
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad ; Pop all backed-up GPRs.
    add esp, 8 ; Pop the interrupt identifier and error code.

    ; Enable interrupts.
    sti

    ; Return from the ISR.
    iret

; Helper macro for defining IRQs.
%macro irq 2
    global irq_%1

    align 4
    irq_%1:

        cli

        push byte 0
        push byte %2

        jmp irq_common_stub
%endmacro

irq 0, 32
irq 1, 33
irq 2, 34
irq 3, 35
irq 4, 36
irq 5, 37
irq 6, 38
irq 7, 39
irq 8, 40
irq 9, 41
irq 10, 42
irq 11, 43
irq 12, 44
irq 13, 45
irq 14, 46
irq 15, 47

; C function for handling IRQs.
extern irq_handler

align 4
irq_common_stub:

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

    ; Expects a signature like: void irq_handler(interrupt_info_t)
    call irq_handler

    ; Reload the original data segment descriptor.
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad ; Pop all backed-up GPRs.
    add esp, 8 ; Pop the interrupt identifier and error code.

    ; Enable interrupts.
    sti

    ; Return from the IRQ.
    iret
