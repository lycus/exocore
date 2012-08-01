#ifndef EXOCORE_H_GDT
#define EXOCORE_H_GDT

#include "exocore/common.h"

typedef struct gdt_entry
{
    ui16 limit_low; // The first 16 bits of the limit.
    ui16 base_low;  // The lower 16 bits of the base.
    ui8 base_middle; // The next 8 bits of the base.
    ui8 access; // Access flags; determine what ring this segment can be used in.
    ui8 flags;
    ui8 base_high; // The last 8 bits of the base.
} attr(packed) gdt_entry_t;

STATIC_ASSERT(sizeof(gdt_entry_t) == 8);

typedef struct gdt_pointer
{
    ui16 limit; // The upper 16 bits of all selector limits.
    gdt_entry_t* base; // The address of the first gdt_entry_t struct.
} attr(packed) gdt_pointer_t;

STATIC_ASSERT(sizeof(gdt_pointer_t) == EXOCORE_IS_32_BIT ? 6 : 10);

#endif
