#include "console.h"
#include "boot/multiboot.h"

void kmain(const ui32 magic, const multiboot_info_t* const info attr(unused))
{
    console_clear();

    if (magic != 0x2badb002)
    {
        console_print(CONSOLE_COLOR_RED, CONSOLE_COLOR_BLACK, "Invalid magic number received from Multiboot bootloader.\n");
        return;
    }

    console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "Kernel booted successfully.\n");

    for (;;)
    {
    }
}
