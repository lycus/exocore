#include "exocore/console.h"
#include "exocore/descriptors.h"
#include "exocore/interrupts.h"
#include "exocore/kernel.h"
#include "exocore/machine.h"
#include "exocore/memory.h"
#include "exocore/multiboot.h"
#include "exocore/timer.h"

ui8* kernel_end_physical;
ui8* kernel_end_virtual;

extern uiptr end;

void kmain(const ui32 magic, const multiboot_info_t* const info, void* const sp)
{
    ASSERT(info);
    ASSERT(sp);

    kernel_end_physical = (ui8*)(&end - KERNEL_LMA);
    kernel_end_virtual = (ui8*)&end;

    CLEAR();

    INFO("Verifying bootloader sanity... ");

    if (magic != MULTIBOOT_KERNEL_MAGIC)
    {
        ERROR("Invalid magic number.\n");
        return;
    }
    else
        SUCCESS("OK.\n");

    initialize_memory();
    initialize_gdt();
    initialize_irq();
    initialize_idt();
    initialize_pit();

    INFO("\n");

    SUCCESS("64-bit kernel booted successfully.\n");

    for (;;)
    {
    }

    disable_interrupts();
    halt_machine();
}
