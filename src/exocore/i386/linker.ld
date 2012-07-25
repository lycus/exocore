OUTPUT_FORMAT("elf32-i386")
ENTRY (i386_loader)

SECTIONS
{
    . = 0x00100000;

    start = .;

    .text ALIGN (0x1000) :
    {
        *(.text)
    }

    etext = .;

    .rodata ALIGN (0x1000) :
    {
        *(.rodata*)
    }

    erodata = .;

    .data ALIGN (0x1000) :
    {
        *(.data)
    }

    edata = .;

    .bss :
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }

    ebss = .;

    end = .;
}
