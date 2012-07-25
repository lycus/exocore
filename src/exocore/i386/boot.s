; The kernel main routine.
extern kmain

; These are defined in the linker script.
extern start
extern edata
extern end

; Export the loader routine (our entry point).
global loader

MAGIC equ 0x1badb002 ; Multiboot magic number.
FLAGS equ 0x00010003 ; Multiboot module flags (we require memory info and module page alignment).
CHECKSUM equ -(MAGIC + FLAGS)
STACK_SIZE equ 0x4000

section .text
align 4

header:

    ; Multiboot-compliant header.
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

    ; Strictly speaking, we would only need these if we defined bit 16 in the
    ; header above. We keep them around just in case.
    dd header
    dd start
    dd edata
    dd end
    dd loader

loader:

    ; Set up the stack (grows down).
    mov esp, stack + STACK_SIZE

    ; Reset EFLAGS.
    push 0
    popf

    ; Enable SSE.
    mov ecx, cr0
    btr ecx, 2      ; clear CR0.EM bit
    bts ecx, 1      ; set CR0.MP bit
    mov cr0, ecx

    mov ecx, cr4
    bts ecx, 9      ; set CR4.OSFXSR bit
    bts ecx, 10     ; set CR4.OSXMMEXCPT bit
    mov cr4, ecx

    ; Pass Multiboot bootloader information.
    ; Expects a signature like: void kmain(ui32, void*)
    push ebx ; Magic number.
    push eax ; Info structure.

    ; Nullify the stack frame pointer.
    xor ebp, ebp

    ; Start the kernel.
    call kmain

    ; Disable interrupt handling.
    cli

.hang:

    ; Make the machine hang if the kernel returns.
    hlt
    jmp .hang

section .bss
align 4

stack: resb STACK_SIZE
