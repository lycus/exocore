OUTPUT_FORMAT("elf64-x86-64")
ENTRY (kernel_loader)

kernel_lma = 0x0000000000100000;
kernel_vma = 0xffff800000000000;

SECTIONS
{
    . = kernel_lma;

    .boot :
    {
        boot = .;

        *(.boot)

        . = ALIGN(0x1000);
    }

    eboot = .;

    . = . + kernel_vma;

    start = .;

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

    end = .;
}
