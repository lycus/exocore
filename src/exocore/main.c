#include "arch.h"
#include "console.h"
#include "boot/multiboot.h"

void kmain(const ui32 magic, const multiboot_info_t* const info attr(unused))
{
    CLEAR();

    INFO("Verifying bootloader sanity... ");

    if (magic != 0x2badb002)
    {
        ERROR("Invalid magic number.\n");
        return;
    }
    else
        SUCCESS("OK.\n");

    INFO("Initializing target architecture... ");

    if (!initialize_architecture())
    {
        ERROR("Failed.\n");
        return;
    }
    else
        SUCCESS("OK.\n");

    INFO("\n");
    SUCCESS("Kernel booted successfully.\n");

    for (;;)
    {
    }
}
