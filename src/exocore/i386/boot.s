; The kernel main routine.
extern kmain

; These are defined in the linker script.
extern start
extern edata
extern end

MAGIC equ 0x1badb002 ; Multiboot magic number.
FLAGS equ 0x00010003 ; Multiboot module flags (we require memory info and module page alignment).
CHECKSUM equ -(MAGIC + FLAGS) ; Fabulous Multiboot checksum.
STACK_SIZE equ 0x4000 ; Size of the kernel execution stack.

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
    dd i386_loader

global i386_loader

i386_loader:

    ; Disable interrupts. We enable interrupts once we
    ; initialize the IDT during target architecture init.
    cli

    ; Set up the stack (grows down).
    mov esp, stack + STACK_SIZE

    ; Pass Multiboot bootloader information.
    push ebx
    push eax

    xor eax, eax

    ; Set I/O permission level to 3.
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

    ; Allow DR4 and DR5 access for compatibility.
    bts ecx, 3 ; Set CR4.DE bit.

    ; Enable saving/restoring of x87 and SSE state.
    bts ecx, 9 ; Set CR4.OSFXSR bit.

    ; Support unmasked SIMD exceptions.
    bts ecx, 10 ; Set CR4.OSXMMEXCPT bit.

    mov cr4, ecx

    ; Nullify the stack frame pointer.
    xor ebp, ebp

    ; Start the kernel. Remember to keep the stack balanced at this point.
    ; Expects a signature like: void kmain(ui32, multiboot_info*)
    call kmain

    ; Disable interrupts once the kernel exits.
    cli

.hang:

    ; Make the machine hang if the kernel returns.
    hlt
    jmp .hang

section .bss

align 4

stack: resb STACK_SIZE
