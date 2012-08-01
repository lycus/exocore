#ifndef EXOCORE_H_GDT
#define EXOCORE_H_GDT

#include "exocore/common.h"

typedef struct gdt_entry
{
    union
    {
        struct
        {
            ui16 limit_low : 16; // The first 16 bits of the limit.
            ui16 base_low : 16;  // The lower 16 bits of the base.
            ui8 base_middle : 8; // The next 8 bits of the base.
            bool accessed : 1;
            bool readable_writable : 1;
            bool direction_conforming : 1;
            bool is_executable : 1;
            bool segment_type : 1;
            ui8 dpl : 2;
            bool present : 1;
            ui8 limit_high : 4;
            bool kernel_bit : 1;
            bool is_long_mode : 1;
            bool is_32_bit : 1;
            bool is_page : 1;
            ui8 base_high : 8; // The last 8 bits of the base.
        } attr(packed);

        ui64 raw;
    };
} attr(packed) gdt_entry_t;

STATIC_ASSERT(sizeof(gdt_entry_t) == 8);

typedef struct gdt_pointer
{
    ui16 limit; // The upper 16 bits of all selector limits.
    gdt_entry_t* base; // The address of the first gdt_entry_t struct.
} attr(packed) gdt_pointer_t;

STATIC_ASSERT(sizeof(gdt_pointer_t) == 10);

#endif
