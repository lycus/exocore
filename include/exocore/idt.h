#ifndef EXOCORE_H_IDT
#define EXOCORE_H_IDT

#include "exocore/common.h"

typedef struct idt_entry
{
    ui16 base_low; // The first 16 bits of the address to jump to when this interrupt fires.
    ui16 selector; // Kernel segment selector.
#if EXOCORE_IS_32_BIT
    ui8 reserved; // This must always be zero.
    ui8 flags; // More flags. See documentation.
    ui16 base_high; // The last 16 bits of the address to jump to.
#else
    ui16 flags; // More flags. See documentation.
    ui16 base_middle; // The next 16 bits of the address to jump to.
    ui32 base_high; // The last 32 bits of the address to jump to.
    ui32 reserved; // This must always be zero.
#endif
} attr(packed) idt_entry_t;

STATIC_ASSERT(sizeof(idt_entry_t) == EXOCORE_IS_32_BIT ? 8 : 16);

typedef struct idt_pointer
{
   ui16 limit;
   idt_entry_t* base; // The address of the first idt_entry_t struct.
} attr(packed) idt_pointer_t;

STATIC_ASSERT(sizeof(idt_pointer_t) == EXOCORE_IS_32_BIT ? 6 : 10);

#endif
