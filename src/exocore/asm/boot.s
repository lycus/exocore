; The kernel main routine.
extern kmain

MULTIBOOT_PAGE_ALIGNMENT equ 1 << 0
MULTIBOOT_MEMORY_INFO equ 1 << 1
MULTIBOOT_VIDEO_MODE equ 1 << 2
MULTIBOOT_AOUT_KLUDGE equ 1 << 16

MULTIBOOT_MAGIC equ 0x1badb002 ; Multiboot magic number.
MULTIBOOT_FLAGS equ MULTIBOOT_PAGE_ALIGNMENT | MULTIBOOT_MEMORY_INFO ; Multiboot module flags.
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS) ; Fabulous Multiboot checksum.

PAGE_SIZE equ 0x1000 ; Size of memory pages.
PAGE_DIRECTORY_ENTRIES equ 1024 ; Number of entries in the page directory.

%ifdef EXOCORE_IS_32_BIT
KERNEL_VMA equ 0xC0000000 ; Keep in sync with the linker script.
%else
KERNEL_VMA equ 0xffff800000000000 ; Keep in sync with the linker script.
%endif

KERNEL_PAGE_NUMBER equ KERNEL_VMA / PAGE_SIZE / PAGE_DIRECTORY_ENTRIES

section .data

align 4096
boot_page_directory:

    dd 00000000000000000000000010000011b
    times KERNEL_PAGE_NUMBER - 1 dd 00000000000000000000000000000000b
    dd 00000000000000000000000010000011b
    times PAGE_DIRECTORY_ENTRIES - KERNEL_PAGE_NUMBER - 2 dd 00000000000000000000000000000000b

section .text

align 8
header:

    ; Multiboot-compliant header.
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

global kernel_loader

align 8
kernel_loader:

    ; Disable interrupts. We enable interrupts once we
    ; initialize the IDT.
    cli

    ; Addresses must be physical until we enable paging.
    mov ecx, boot_page_directory
    mov edx, KERNEL_VMA
    sub ecx, edx

    ; Set the page directory.
    mov cr3, ecx

    mov ecx, cr4

    ; Enable 4 MB pages.
    bts ecx, 4 ; Set CR4.PSE bit.

    mov cr4, ecx

    mov ecx, cr0

    ; Enable paging. We are already in protected mode, so
    ; we do not need to set CR0.PE.
    bts ecx, 31 ; Set CR0.PG bit.

    mov cr0, ecx

    mov ecx, cr4

    ; Enable global paging.
    bts ecx, 7 ; Set CR4.PGE bit.

    mov cr4, ecx

    ; Get to the higher half (kernel space).
    lea ecx, [.high]
    jmp ecx

align 8
.high:

    ; Set up the stack (grows down).
    mov esp, stack_top

    ; Pass Multiboot bootloader information and stack pointer.
    push esp
    push ebx
    push eax

    ; Reset EFLAGS to a known state.
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

    ; Disable write protection.
    btr ecx, 16 ; Clear CR0.WP bit.

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
    ; Expects a signature like: void kmain(ui32, multiboot_info*, void*)
    call kmain

    ; If the C code for some reason returns, just halt.
    hlt

section .bss

align 8
stack_bottom:

    resb 0x4000

stack_top:
