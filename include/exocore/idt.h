#ifndef EXOCORE_H_IDT
#define EXOCORE_H_IDT

#include "exocore/common.h"

typedef struct idt_entry
{
    union
    {
        struct
        {
            ui16 base_low : 16;
            ui16 selector : 16;
            ui8 ist : 3;
            ui8 unused1 : 5;
            ui8 type : 4;
            bool unused2 : 1;
            ui8 dpl : 2;
            bool present : 1;
            ui16 base_middle : 16;
            ui32 base_high : 32;
            ui32 unused3 : 32;
        } attr(packed);

        ui64 raw1;
        ui64 raw2;
    };
} attr(packed) idt_entry_t;

STATIC_ASSERT(sizeof(idt_entry_t) == 16);

typedef struct idt_pointer
{
   ui16 limit;
   idt_entry_t* base; // The address of the first idt_entry_t struct.
} attr(packed) idt_pointer_t;

STATIC_ASSERT(sizeof(idt_pointer_t) == 10);

#endif
