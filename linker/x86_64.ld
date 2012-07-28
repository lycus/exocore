OUTPUT_FORMAT("elf64-x86-64")
ENTRY (kernel_loader)

kernel_lma = 0x0000000000100000;
kernel_vma = 0xffff800000000000;

SECTIONS
{
    . = kernel_vma + kernel_lma;

    start = .;

    .text : AT(ADDR(.text) - kernel_vma)
    {
        *(.text)

        . = ALIGN(0x1000);
    }

    .data : AT(ADDR(.data) - kernel_vma)
    {
        *(.data)

        . = ALIGN(0x1000);
    }

    .rodata : AT(ADDR(.rodata) - kernel_vma)
    {
        *(.rodata*)

        . = ALIGN(0x1000);
    }

    .bss : AT(ADDR(.bss) - kernel_vma)
    {
        *(.bss)

        . = ALIGN(0x1000);
    }

    end = .;
}
