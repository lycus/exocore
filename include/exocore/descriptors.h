#ifndef EXOCORE_H_DESCRIPTORS
#define EXOCORE_H_DESCRIPTORS

#include "exocore/common.h"

typedef struct gdt_entry
{
    ui16 limit_low; // The lower 16 bits of the limit.
    ui16 base_low;  // The lower 16 bits of the base.
    ui8 base_middle; // The next 8 bits of the base.
    ui8 access; // Access flags; determine what ring this segment can be used in.
    ui8 granularity;
    ui8 base_high; // The last 8 bits of the base.
} attr(packed) gdt_entry_t;

typedef struct gdt_pointer
{
    ui16 limit; // The upper 16 bits of all selector limits.
    gdt_entry_t* base; // The address of the first gdt_entry_t struct.
} attr(packed) gdt_pointer_t;

typedef struct idt_entry
{
    ui16 base_low; // The lower 16 bits of the address to jump to when this interrupt fires.
    ui16 selector; // Kernel segment selector.
    ui8 reserved; // This must always be zero.
    ui8 flags; // More flags. See documentation.
    ui16 base_high; // The upper 16 bits of the address to jump to.
} attr(packed) idt_entry_t;

typedef struct idt_pointer
{
   ui16 limit;
   idt_entry_t* base; // The address of the first idt_entry_t struct.
} attr(packed) idt_pointer_t;

// Sets up the GDT.
void initialize_gdt(void);

// Remaps IRQs.
void initialize_irq(void);

// Sets up the IDT. Should be called after initialize_irq.
void initialize_idt(void);

// Number of entries in the global descriptor table.
#define GDT_SIZE 5

// Number of entries in the interrupt descriptor table.
#define IDT_SIZE 256

#endif
