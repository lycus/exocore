OUTPUT_FORMAT("elf32-i386")
ENTRY (kernel_loader)

kernel_lma = 0x00100000;
kernel_vma = 0xC0000000;

SECTIONS
{
    . = kernel_vma + kernel_lma;

    .boot : AT(ADDR(.boot) - kernel_vma)
    {
        boot = .;

        *(.boot)

        . = ALIGN(0x1000);
    }

    eboot = .;

    .text : AT(ADDR(.text) - kernel_vma)
    {
        text = .;

        *(.text)

        . = ALIGN(0x1000);
    }

    etext = .;

    .data : AT(ADDR(.data) - kernel_vma)
    {
        data = .;

        *(.data)

        . = ALIGN(0x1000);
    }

    edata = .;

    .rodata : AT(ADDR(.rodata) - kernel_vma)
    {
        rodata = .;

        *(.rodata*)

        . = ALIGN(0x1000);
    }

    erodata = .;

    .bss : AT(ADDR(.bss) - kernel_vma)
    {
        bss = .;

        *(.bss)

        . = ALIGN(0x1000);
    }

    ebss = .;
}
