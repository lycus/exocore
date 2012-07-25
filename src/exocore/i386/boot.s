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

    ; Pass Multiboot bootloader information.
    ; Expects a signature like: void kmain(ui32, void*)
    push ebx,
    push eax

    ; Initialize EFLAGS and set I/O permission level to 3.
    xor eax, eax
    bts eax, 12 ; Set EFLAGS.IOPL.1 bit.
    bts eax, 13 ; Set EFLAGS.IOPL.2 bit.
    push eax
    popf

    mov ecx, cr0

    ; Enable x87 and SSE.
    bts ecx, 1 ; Set CR0.MP bit.
    btr ecx, 2 ; Clear CR0.EM bit.

    ; Ensure memory coherency is maintained.
    btr ecx, 29 ; Clear CR0.NW bit.
    bts ecx, 30 ; Set CR0.CD bit.

    mov cr0, ecx

    mov ecx, cr4
    bts ecx, 9      ; set CR4.OSFXSR bit
    bts ecx, 10     ; set CR4.OSXMMEXCPT bit
    mov cr4, ecx

    ; Nullify the stack frame pointer.
    xor ebp, ebp

    ; Start the kernel. Remember to keep the stack balanced at this point.
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
