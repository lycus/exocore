#include "exocore/arch.h"
#include "exocore/console.h"
#include "exocore/boot/multiboot.h"

void kmain(const ui32 magic, const multiboot_info_t* const info)
{
    ASSERT(info);

    CLEAR();

    INFO("Verifying bootloader sanity... ");

    if (magic != 0x2badb002)
    {
        ERROR("Invalid magic number.\n");
        return;
    }
    else
        SUCCESS("OK.\n");

    initialize_architecture();

    INFO("\n");
    SUCCESS("Kernel booted successfully.\n");

    for (;;)
    {
    }

    disable_interrupts();
    halt_machine();
}
