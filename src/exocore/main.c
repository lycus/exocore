#include "boot/multiboot.h"

void kmain(const ui32 magic, const multiboot_info_t* const info attr(unused))
{
    // TODO: Print error.
    if (magic != 0x2badb002)
        return;

    volatile ui8* const video = (volatile ui8* const)0xB8000;

    video[0] = 65;
    video[1] = 0x07;

    for (;;)
    {
    }
}
