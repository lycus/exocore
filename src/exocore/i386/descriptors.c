#include "exocore/console.h"
#include "exocore/io.h"
#include "exocore/i386/descriptors.h"
#include "exocore/i386/pic.h"

static gdt_entry_t gdt[GDT_SIZE] attr(aligned(8)); // The global descriptor table.
static gdt_pointer_t gdt_pointer attr(aligned(8)); // Pointer to the GDT.

static idt_entry_t idt[IDT_SIZE] attr(aligned(8)); // The interrupt descriptor table.
static idt_pointer_t idt_pointer attr(aligned(8)); // Pointer to the IDT.

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

// Interrupt request routines defined in assembly land.

extern void i386_irq_0(void);
extern void i386_irq_1(void);
extern void i386_irq_2(void);
extern void i386_irq_3(void);
extern void i386_irq_4(void);
extern void i386_irq_5(void);
extern void i386_irq_6(void);
extern void i386_irq_7(void);
extern void i386_irq_8(void);
extern void i386_irq_9(void);
extern void i386_irq_10(void);
extern void i386_irq_11(void);
extern void i386_irq_12(void);
extern void i386_irq_13(void);
extern void i386_irq_14(void);
extern void i386_irq_15(void);

#define ISR_CPU_START 0
#define ISR_IRQ_MASTER_START 32
#define ISR_IRQ_SLAVE_START 40

static void remap_irq(void)
{
    INFO("Remapping i386 IRQs... ");

    io_write_ui8(PIC_MASTER_COMMAND, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    io_wait();
    io_write_ui8(PIC_SLAVE_COMMAND, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    io_wait();

    io_write_ui8(PIC_MASTER_DATA, ISR_IRQ_MASTER_START);
    io_wait();
    io_write_ui8(PIC_SLAVE_DATA, ISR_IRQ_SLAVE_START);
    io_wait();

    io_write_ui8(PIC_MASTER_DATA, 0x04);
    io_wait();
    io_write_ui8(PIC_SLAVE_DATA, 0x02);
    io_wait();

    io_write_ui8(PIC_MASTER_DATA, PIC_ICW4_8086);
    io_wait();
    io_write_ui8(PIC_SLAVE_DATA, PIC_ICW4_8086);
    io_wait();

    io_write_ui8(PIC_MASTER_DATA, 0x00);
    io_wait();
    io_write_ui8(PIC_SLAVE_DATA, 0x00);
    io_wait();

    SUCCESS("OK.\n");
}

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
    INFO("Initializing i386 global descriptor table... ");

    gdt_pointer.limit = sizeof(gdt_entry_t) * GDT_SIZE - 1;
    gdt_pointer.base = &gdt[0];

    gdt_set_descriptor(0, 0, 0x00000000, 0x00, 0x00);  // Null segment.
    gdt_set_descriptor(1, 0, 0xffffffff, 0x9a, 0xcf); // Kernel mode code segment.
    gdt_set_descriptor(2, 0, 0xffffffff, 0x92, 0xcf); // Kernel mode data segment.
    gdt_set_descriptor(3, 0, 0xffffffff, 0xfa, 0xcf); // User mode code segment.
    gdt_set_descriptor(4, 0, 0xffffffff, 0xf2, 0xcf); // User mode data segment.

    i386_gdt_flush(&gdt_pointer);

    SUCCESS("OK.\n");
}

static void idt_set_descriptor(uiptr index, void (* base)(void), ui16 selector, ui8 flags)
{
    ASSERT(base);

    uiptr addr = (uiptr)base;

    idt[index].base_low = addr & 0xffff;
    idt[index].base_high = (addr >> 16) & 0xffff;

    idt[index].selector = selector;

    idt[index].reserved = 0;

    idt[index].flags = flags | 0x60; // Enable user mode interrupts.
}

static void initialize_idt(void)
{
    INFO("Initializing i386 interrupt descriptor table... ");

    idt_pointer.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idt_pointer.base = &idt[0];

    idt_set_descriptor(ISR_CPU_START, &i386_isr_0, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 1, &i386_isr_1, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 2, &i386_isr_2, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 3, &i386_isr_3, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 4, &i386_isr_4, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 5, &i386_isr_5, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 6, &i386_isr_6, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 7, &i386_isr_7, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 8, &i386_isr_8, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 9, &i386_isr_9, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 10, &i386_isr_10, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 11, &i386_isr_11, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 12, &i386_isr_12, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 13, &i386_isr_13, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 14, &i386_isr_14, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 15, &i386_isr_15, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 16, &i386_isr_16, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 17, &i386_isr_17, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 18, &i386_isr_18, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 19, &i386_isr_19, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 20, &i386_isr_20, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 21, &i386_isr_21, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 22, &i386_isr_22, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 23, &i386_isr_23, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 24, &i386_isr_24, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 25, &i386_isr_25, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 26, &i386_isr_26, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 27, &i386_isr_27, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 28, &i386_isr_28, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 29, &i386_isr_29, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 30, &i386_isr_30, 0x0008, 0x8e);
    idt_set_descriptor(ISR_CPU_START + 31, &i386_isr_31, 0x0008, 0x8e);

    idt_set_descriptor(ISR_IRQ_MASTER_START, &i386_irq_0, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 1, &i386_irq_1, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 2, &i386_irq_2, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 3, &i386_irq_3, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 4, &i386_irq_4, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 5, &i386_irq_5, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 6, &i386_irq_6, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 7, &i386_irq_7, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START, &i386_irq_8, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 1, &i386_irq_9, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 2, &i386_irq_10, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 3, &i386_irq_11, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 4, &i386_irq_12, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 5, &i386_irq_13, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 6, &i386_irq_14, 0x0008, 0x8e);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 7, &i386_irq_15, 0x0008, 0x8e);

    i386_idt_flush(&idt_pointer);

    SUCCESS("OK.\n");
}

void i386_initialize_descriptors(void)
{
    remap_irq();
    initialize_gdt();
    initialize_idt();
}
