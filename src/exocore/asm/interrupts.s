%include "exocore/asm/segments.s"

section .text

global enable_interrupts

align 8
enable_interrupts:

    sti
    ret

global disable_interrupts

align 8
disable_interrupts:

    cli
    ret

; Helper macro for defining ISRs with no error codes.
%macro isr_no_error 1
    global isr_%1

    align 8
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

    align 8
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

align 8
isr_common_stub:

    ; Save all GPRs.
    push rax
    push rcx
    push rdx
    push rbx
    push rsp
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Expects a signature like: void isr_handler(interrupt_info_t*)
    mov rdi, rsp
    call isr_handler

    ; Restore all GPRs.
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax

    ; Pop the interrupt identifier and error code.
    add esp, 8 + 8

    ; Enable interrupts.
    sti

    ; Return from the ISR.
    iretq

; Helper macro for defining IRQs.
%macro irq 2
    global irq_%1

    align 8
    irq_%1:

        cli

        push 0
        push %2

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

align 8
irq_common_stub:

    ; Save all GPRs.
    push rax
    push rcx
    push rdx
    push rbx
    push rsp
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Expects a signature like: void irq_handler(interrupt_info_t*)
    mov rdi, rsp
    call irq_handler

    ; Restore all GPRs.
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax

    ; Pop the interrupt identifier and error code.
    add esp, 8 + 8

    ; Enable interrupts.
    sti

    ; Return from the IRQ.
    iretq
