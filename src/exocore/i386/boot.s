; The kernel main routine.
extern kmain

; These are defined in the linker script.
extern start
extern edata
extern end

; Export the loader routine (our entry point).
global i386_loader

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
    dd i386_loader

i386_loader:

    ; Disable interrupts.
    cli

    ; Set up the stack (grows down).
    mov esp, stack + STACK_SIZE

    ; Pass Multiboot bootloader information.
    ; Expects a signature like: void kmain(ui32, void*)
    push ebx,
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

    ; Set up the GDT.
    lgdt [gdt]

    ; Nullify the stack frame pointer.
    xor ebp, ebp

    ; Start the kernel. Remember to keep the stack balanced at this point.
    call kmain

.hang:

    ; Make the machine hang if the kernel returns.
    hlt
    jmp .hang

align 4096 ; The GDT must be aligned on a page boundary.

gdt:

    dw gdt_end - gdt_table - 1
    dd gdt_table

gdt_table:

    struc gdt_entry_struc

        limit_low: resb 2
        base_low: resb 2
        base_middle: resb 1
        access: resb 1
        granularity: resb 1
        base_high: resb 1

    endstruc

    ; The null descriptor.
    istruc gdt_entry_struc

        at limit_low, db 0, 0
        at base_low, db 0, 0
        at base_middle, db 0
        at access, db 0
        at granularity, db 0
        at base_high, db 0

    iend

    ; TODO: Add the rest of the entries.

gdt_end:

section .bss

align 4

stack: resb STACK_SIZE
