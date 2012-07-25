#include "ssp.h"
#include "boot/multiboot.h"

// Main entry point of the kernel.
void kmain(const ui32 magic, const multiboot_info_t* const info attr(unused))
{
    // Set up the SSP. Should be done as early as
    // soon as we execute any C code (i.e. here).
    ssp_setup();

    // TODO: Print error.
    if (magic != 0x2badb002)
        return;

    for (;;)
    {
    }
}
