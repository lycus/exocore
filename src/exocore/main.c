#include "exocore/console.h"
#include "exocore/descriptors.h"
#include "exocore/interrupts.h"
#include "exocore/machine.h"
#include "exocore/multiboot.h"
#include "exocore/timer.h"

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

    initialize_gdt();
    initialize_irq();
    initialize_idt();
    initialize_pit();

    INFO("\n");
    SUCCESS("Kernel booted successfully.\n");

    for (;;)
    {
    }

    disable_interrupts();
    halt_machine();
}
