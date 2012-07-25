#include "console.h"
#include "boot/multiboot.h"

void kmain(const ui32 magic, const multiboot_info_t* const info attr(unused))
{
    // TODO: Print error.
    if (magic != 0x2badb002)
        return;

    console_clear();

    console_print(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_BLACK, "Kernel booted.\n");

    for (;;)
    {
    }
}
