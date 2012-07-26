#include "i386/descriptors.h"

#define GDT_SIZE 5
#define IDT_SIZE 31

static gdt_entry_t gdt[GDT_SIZE]; // The global descriptor table.
static gdt_pointer_t gdt_pointer; // Pointer to the GDT.

static idt_entry_t idt[IDT_SIZE]; // The interrupt descriptor table.
static idt_pointer_t idt_pointer; // Pointer to the IDT.

// These functions are used to activate new GDTs/IDTs.

attr(nonnull) extern void i386_gdt_flush(gdt_pointer_t* gdt);
attr(nonnull) extern void i386_idt_flush(idt_pointer_t* idt);

// Interrupt service routines defined in assembly land.

extern void i386_isr_0(void);
extern void i386_isr_1(void);
extern void i386_isr_2(void);
extern void i386_isr_3(void);
extern void i386_isr_4(void);
extern void i386_isr_5(void);
extern void i386_isr_6(void);
extern void i386_isr_7(void);
extern void i386_isr_8(void);
extern void i386_isr_9(void);
extern void i386_isr_10(void);
extern void i386_isr_11(void);
extern void i386_isr_12(void);
extern void i386_isr_13(void);
extern void i386_isr_14(void);
extern void i386_isr_15(void);
extern void i386_isr_16(void);
extern void i386_isr_17(void);
extern void i386_isr_18(void);
extern void i386_isr_19(void);
extern void i386_isr_20(void);
extern void i386_isr_21(void);
extern void i386_isr_22(void);
extern void i386_isr_23(void);
extern void i386_isr_24(void);
extern void i386_isr_25(void);
extern void i386_isr_26(void);
extern void i386_isr_27(void);
extern void i386_isr_28(void);
extern void i386_isr_29(void);
extern void i386_isr_30(void);
extern void i386_isr_31(void);

static void gdt_set_descriptor(uiptr index, ui32 base, ui32 limit, ui8 access, ui8 granularity)
{
    gdt[index].base_low = base & 0xffff;
    gdt[index].base_middle = (base >> 16) & 0xff;
    gdt[index].base_high = (base >> 24) & 0xff;

    gdt[index].limit_low = limit & 0xffff;

    gdt[index].granularity = (limit >> 16) & 0x0f;
    gdt[index].granularity |= granularity & 0xf0;

    gdt[index].access = access;
}

static void initialize_gdt(void)
{
    gdt_pointer.limit = sizeof(gdt_entry_t) * GDT_SIZE - 1;
    gdt_pointer.base = &gdt[0];

    gdt_set_descriptor(0, 0, 0x00000000, 0x00, 0x00);  // Null segment.
    gdt_set_descriptor(1, 0, 0xffffffff, 0x9a, 0xcf); // Kernel mode code segment.
    gdt_set_descriptor(2, 0, 0xffffffff, 0x92, 0xcf); // Kernel mode data segment.
    gdt_set_descriptor(3, 0, 0xffffffff, 0xfa, 0xcf); // User mode code segment.
    gdt_set_descriptor(4, 0, 0xffffffff, 0xf2, 0xcf); // User mode data segment.

    i386_gdt_flush(&gdt_pointer);
}

static void idt_set_descriptor(uiptr index, void (*base)(void), ui16 selector, ui8 flags)
{
    uiptr addr = (uiptr)base;

    idt[index].base_low = addr & 0xffff;
    idt[index].base_high = (addr >> 16) & 0xffff;

    idt[index].selector = selector;

    idt[index].reserved = 0;

    idt[index].flags = flags | 0x60; // Enable user mode interrupts.
}

static void initialize_idt(void)
{
    idt_pointer.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idt_pointer.base = &idt[0];

    idt_set_descriptor(0, &i386_isr_0, 0x0008, 0x8e);
    idt_set_descriptor(1, &i386_isr_1, 0x0008, 0x8e);
    idt_set_descriptor(2, &i386_isr_2, 0x0008, 0x8e);
    idt_set_descriptor(3, &i386_isr_3, 0x0008, 0x8e);
    idt_set_descriptor(4, &i386_isr_4, 0x0008, 0x8e);
    idt_set_descriptor(5, &i386_isr_5, 0x0008, 0x8e);
    idt_set_descriptor(6, &i386_isr_6, 0x0008, 0x8e);
    idt_set_descriptor(7, &i386_isr_7, 0x0008, 0x8e);
    idt_set_descriptor(8, &i386_isr_8, 0x0008, 0x8e);
    idt_set_descriptor(9, &i386_isr_9, 0x0008, 0x8e);
    idt_set_descriptor(10, &i386_isr_10, 0x0008, 0x8e);
    idt_set_descriptor(11, &i386_isr_11, 0x0008, 0x8e);
    idt_set_descriptor(12, &i386_isr_12, 0x0008, 0x8e);
    idt_set_descriptor(13, &i386_isr_13, 0x0008, 0x8e);
    idt_set_descriptor(14, &i386_isr_14, 0x0008, 0x8e);
    idt_set_descriptor(15, &i386_isr_15, 0x0008, 0x8e);
    idt_set_descriptor(16, &i386_isr_16, 0x0008, 0x8e);
    idt_set_descriptor(17, &i386_isr_17, 0x0008, 0x8e);
    idt_set_descriptor(18, &i386_isr_18, 0x0008, 0x8e);
    idt_set_descriptor(19, &i386_isr_19, 0x0008, 0x8e);
    idt_set_descriptor(20, &i386_isr_20, 0x0008, 0x8e);
    idt_set_descriptor(21, &i386_isr_21, 0x0008, 0x8e);
    idt_set_descriptor(22, &i386_isr_22, 0x0008, 0x8e);
    idt_set_descriptor(23, &i386_isr_23, 0x0008, 0x8e);
    idt_set_descriptor(24, &i386_isr_24, 0x0008, 0x8e);
    idt_set_descriptor(25, &i386_isr_25, 0x0008, 0x8e);
    idt_set_descriptor(26, &i386_isr_26, 0x0008, 0x8e);
    idt_set_descriptor(27, &i386_isr_27, 0x0008, 0x8e);
    idt_set_descriptor(28, &i386_isr_28, 0x0008, 0x8e);
    idt_set_descriptor(29, &i386_isr_29, 0x0008, 0x8e);
    idt_set_descriptor(30, &i386_isr_30, 0x0008, 0x8e);
    idt_set_descriptor(31, &i386_isr_31, 0x0008, 0x8e);

    i386_idt_flush(&idt_pointer);
}

void i386_initialize_descriptors(void)
{
    initialize_gdt();
    initialize_idt();
}
