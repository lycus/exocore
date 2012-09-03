%include "exocore/asm/boot.s"
%include "exocore/asm/multiboot.s"
%include "exocore/asm/segments.s"

; The kernel main routine.
extern kmain

section .boot
bits 32

align 8
header:

    ; Multiboot 2-compliant header.
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_ARCHITECTURE
    dd header_end - header
    dd -(MULTIBOOT_MAGIC + MULTIBOOT_ARCHITECTURE + (header_end - header))

module_alignment:

    dw MULTIBOOT_TAG_MODULE_ALIGNMENT
    dw 0000000000000000b
    dd module_alignment_end - module_alignment

module_alignment_end:

tag_end:

    ; Terminate tag list.
    dw MULTIBOOT_TAG_END
    dw 0000000000000000b
    dd tag_end_end - tag_end

tag_end_end:

header_end:

align PAGE_SIZE
pml4_base:

    dq pml3_base + 0x0000000000000007
    times 255 dq 0
    dq pml3_base + 0x0000000000000007
    times 253 dq 0
    dq pml4_base + 0x0000000000000007
    dq 0

align PAGE_SIZE
pml3_base:

    dq pml2_base + 0x0000000000000007
    times 511 dq 0

align PAGE_SIZE
pml2_base:

%assign i 0
%rep 50
    dq pml1_base + i + 0x0000000000000007
%assign i i + PAGE_SIZE
%endrep

    times (512 - 50) dq 0

align PAGE_SIZE
pml1_base:

%assign i 0
%rep 512 * 50
    dq (i << 12) | 0x0000000000000087
%assign i i + 1
%endrep

align 8
boot_gdt_pointer:

    dw boot_gdt_end - boot_gdt - 1
    dq boot_gdt - KERNEL_VMA

align 8
boot_gdt:

    ; Null segment.
    dw 0000000000000000b
    dw 0000000000000000b
    db 00000000b
    db 00000000b
    db 00000000b
    db 00000000b
    ; 64-bit kernel code segment.
    dw 0000000000000000b
    dw 0000000000000000b
    db 00000000b
    db 10011000b
    db 00100000b
    db 00000000b
    ; 64-bit kernel data segment.
    dw 0000000000000000b
    dw 0000000000000000b
    db 00000000b
    db 10010000b
    db 00000000b
    db 00000000b

boot_gdt_end:

global kernel_loader

align 8
kernel_loader:

    ; Disable interrupts. We enable interrupts once we set
    ; up the real IDT.
    cli

    ; Back up Multiboot information.
    mov edi, eax ; Magic number.
    mov esi, ebx ; Info structure pointer.

    mov ecx, cr4

    ; Enable 64-bit page translation table entries.
    bts ecx, 5 ; Set CR4.PAE bit.

    mov cr4, ecx

    ; Set up the long mode page table.
    mov ecx, pml4_base
    mov cr3, ecx

    ; Enable long mode.
    mov ecx, 0xc0000080
    rdmsr
    bts eax, 8 ; Set IA32_EFER.LME bit.
    wrmsr

    ; Load the boot-time GDT.
    lgdt [boot_gdt_pointer]

    mov ecx, cr0

    ; Enable paging. We are already in protected mode, so
    ; we do not need to set CR0.PE.
    bts ecx, 31 ; Set CR0.PG bit.

    mov cr0, ecx

    jmp KERNEL_CODE_SEGMENT:high64 - KERNEL_VMA

section .text

bits 64

align 8
stack_bottom:

    times 0x4000 db 0

stack_top:

align 8
high64:

    ; Establish a 32-bit stack.
    mov rsp, stack_top - KERNEL_VMA

    push KERNEL_CODE_SEGMENT

    mov rax, (KERNEL_VMA >> 32) << 32
    or rax, high - (KERNEL_VMA & 0xffffffff00000000)

    push rax

    ret

align 8
high:

    ; Set up the stack (grows down).
    mov rsp, (KERNEL_VMA >> 32) << 32
    or rsp, stack_top - (KERNEL_VMA & 0xffffffff00000000)

    ; Reset EFLAGS to a known state.
    xor eax, eax

    ; Set I/O permission level to 3.
    bts eax, 12 ; Set EFLAGS.IOPL.1 bit.
    bts eax, 13 ; Set EFLAGS.IOPL.2 bit.

    push rax
    popfq

    mov rcx, cr0

    ; Enable x87 and SSE.
    bts rcx, 1 ; Set CR0.MP bit.
    btr rcx, 2 ; Clear CR0.EM bit.

    ; Disable write protection.
    btr rcx, 16 ; Clear CR0.WP bit.

    ; Ensure memory coherency is maintained.
    btr rcx, 29 ; Clear CR0.NW bit.
    bts rcx, 30 ; Set CR0.CD bit.

    mov cr0, rcx

    mov rcx, cr4

    ; Allow DR4 and DR5 access for compatibility.
    bts rcx, 3 ; Set CR4.DE bit.

    ; Enable saving/restoring of x87 and SSE state.
    bts rcx, 9 ; Set CR4.OSFXSR bit.

    ; Support unmasked SIMD exceptions.
    bts rcx, 10 ; Set CR4.OSXMMEXCPT bit.

    mov cr4, rcx

    ; Nullify the stack frame pointer.
    xor rbp, rbp

    ; Pass Multiboot information and stack pointer.
    ; RDI and RSI are already set.
    mov rdx, rsp

    ; Start the kernel. Expects a signature like: void kmain(ui32, multiboot_info*, void*)
    call kmain

    ; If the C code for some reason returns, just halt.
    hlt
