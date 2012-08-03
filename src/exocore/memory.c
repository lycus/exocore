#include "exocore/kernel.h"
#include "exocore/memory.h"
#include "exocore/paging.h"

static ui8* virtual_placement_address;
static ui8* physical_placement_address;

void initialize_memory(void)
{
    virtual_placement_address = kernel_end_virtual;
    physical_placement_address = kernel_end_physical;
}

uiptr* allocate(const uiptr size, const bool page_align, uiptr** const physical_address)
{
    ASSERT(size);

    // TODO: Allocate from the heap when it's set up.

    // Align to either a machine word or page boundary.
    virtual_placement_address = (ui8*)align_to((uiptr)virtual_placement_address, page_align ? PAGE_SIZE : sizeof(uiptr));
    physical_placement_address = (ui8*)align_to((uiptr)physical_placement_address, page_align ? PAGE_SIZE : sizeof(uiptr));

    if (physical_address)
        *physical_address = (uiptr*)physical_placement_address;

    ui8* addr = virtual_placement_address;

    virtual_placement_address += size;
    physical_placement_address += size;

    return (uiptr*)addr;
}

void free(const void* const data)
{
    ASSERT(data);
    ASSERT(is_aligned((uiptr)data, sizeof(uiptr)));

    // TODO: Free memory if the heap is set up.
}
