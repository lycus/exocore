#include "arch.h"
#include "console.h"
#include "boot/multiboot.h"

void kmain(const ui32 magic, const multiboot_info_t* const info attr(unused))
{
    console_clear();

    console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "Verifying bootloader sanity... ");

    if (magic != 0x2badb002)
    {
        console_print(CONSOLE_COLOR_RED, CONSOLE_COLOR_BLACK, "Invalid magic number.\n");
        return;
    }
    else
        console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "OK.\n");

    console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "Initializing target architecture... ");

    if (!initialize_architecture())
    {
        console_print(CONSOLE_COLOR_RED, CONSOLE_COLOR_BLACK, " Failed.\n");
        return;
    }
    else
        console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "OK.\n");

    console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "\n");
    console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "Kernel booted successfully.\n");

    for (;;)
    {
    }
}
