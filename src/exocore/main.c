#include "exocore/arch.h"
#include "exocore/console.h"
#include "exocore/boot/multiboot.h"

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

    bool result = initialize_architecture();

    INFO("Initializing target architecture... ");

    if (!result)
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

    disable_interrupts();
    halt_machine();
}
