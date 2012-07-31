#include "exocore/builtins.h"
#include "exocore/console.h"
#include "exocore/descriptors.h"
#include "exocore/interrupts.h"
#include "exocore/io.h"
#include "exocore/pic.h"

static gdt_entry_t gdt[GDT_SIZE] attr(aligned(8)); // The global descriptor table.
static gdt_pointer_t gdt_pointer attr(aligned(8)); // Pointer to the GDT.

static idt_entry_t idt[IDT_SIZE] attr(aligned(8)); // The interrupt descriptor table.
static idt_pointer_t idt_pointer attr(aligned(8)); // Pointer to the IDT.

// These functions are used to activate new GDTs/IDTs.

attr(nonnull) extern void gdt_flush(const gdt_pointer_t* const gdt);
attr(nonnull) extern void idt_flush(const idt_pointer_t* const idt);

// Interrupt service routines defined in assembly land.

extern void isr_0(void);
extern void isr_1(void);
extern void isr_2(void);
extern void isr_3(void);
extern void isr_4(void);
extern void isr_5(void);
extern void isr_6(void);
extern void isr_7(void);
extern void isr_8(void);
extern void isr_9(void);
extern void isr_10(void);
extern void isr_11(void);
extern void isr_12(void);
extern void isr_13(void);
extern void isr_14(void);
extern void isr_15(void);
extern void isr_16(void);
extern void isr_17(void);
extern void isr_18(void);
extern void isr_19(void);
extern void isr_20(void);
extern void isr_21(void);
extern void isr_22(void);
extern void isr_23(void);
extern void isr_24(void);
extern void isr_25(void);
extern void isr_26(void);
extern void isr_27(void);
extern void isr_28(void);
extern void isr_29(void);
extern void isr_30(void);
extern void isr_31(void);

// Interrupt request routines defined in assembly land.

extern void irq_0(void);
extern void irq_1(void);
extern void irq_2(void);
extern void irq_3(void);
extern void irq_4(void);
extern void irq_5(void);
extern void irq_6(void);
extern void irq_7(void);
extern void irq_8(void);
extern void irq_9(void);
extern void irq_10(void);
extern void irq_11(void);
extern void irq_12(void);
extern void irq_13(void);
extern void irq_14(void);
extern void irq_15(void);

#define ISR_CPU_START 0
#define ISR_IRQ_MASTER_START 32
#define ISR_IRQ_SLAVE_START 40

void initialize_irq(void)
{
    INFO("Remapping interrupt requests... ");

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

static void gdt_set_descriptor(const uiptr index, const ui32 base, const ui32 limit, const ui8 access, const ui8 flags)
{
    ASSERT(index < GDT_SIZE);
    ASSERT(limit ? builtin_clz32(limit) >= bitsof(limit) - 20 : true);
    ASSERT(flags ? builtin_clz32(flags) >= bitsof(flags) - 4 : true);

    gdt_entry_t* const entry = &gdt[index];

    entry->base_low = base & 0x0000ffff;
    entry->base_middle = (base >> 16) & 0x000000ff;
    entry->base_high = (base >> 24) & 0x000000ff;

    entry->limit_low = limit & 0x0000ffff;

    entry->flags = (ui8)((flags & 0x0f) << 4) | (limit >> 16 & 0x0000000f);

    entry->access = access;
}

void initialize_gdt(void)
{
    INFO("Initializing global descriptor table... ");

    gdt_pointer.limit = sizeof(gdt_entry_t) * GDT_SIZE - 1;
    gdt_pointer.base = &gdt[0];

#if EXOCORE_IS_32_BIT
    gdt_set_descriptor(0, 0x00000000, 0x00000000, 0b00000000, 0b00000000); // Null segment.
    gdt_set_descriptor(1, 0x00000000, 0x000fffff, 0b10011000, 0b00001100); // Kernel mode code segment.
    gdt_set_descriptor(2, 0x00000000, 0x000fffff, 0b10010010, 0b00001100); // Kernel mode data segment.
    gdt_set_descriptor(3, 0x00000000, 0x000fffff, 0b11111000, 0b00001100); // User mode code segment.
    gdt_set_descriptor(4, 0x00000000, 0x000fffff, 0b11110010, 0b00001100); // User mode data segment.
#else
    gdt_set_descriptor(0, 0x00000000, 0x00000000, 0b00000000, 0b00000000); // Null segment.
    gdt_set_descriptor(1, 0x00000000, 0x00000000, 0b10011000, 0b00000010); // Kernel mode code segment.
    gdt_set_descriptor(2, 0x00000000, 0x00000000, 0b10010010, 0b00000010); // Kernel mode data segment.
    gdt_set_descriptor(3, 0x00000000, 0x00000000, 0b11111000, 0b00000010); // User mode code segment.
    gdt_set_descriptor(4, 0x00000000, 0x00000000, 0b11110010, 0b00000010); // User mode data segment.
#endif

    gdt_flush(&gdt_pointer);

    SUCCESS("OK.\n");
}

static void idt_set_descriptor(const uiptr index, void (* const base)(void), const ui16 selector, const ui16 flags)
{
    ASSERT(index < IDT_SIZE);
    ASSERT(base);
    ASSERT(!((uiptr)base % sizeof(uiptr)));
    ASSERT(selector);
    ASSERT(!(selector % sizeof(gdt_entry_t)));
#if EXOCORE_IS_32_BIT
    ASSERT(!(flags & 0b00010001));
    ASSERT(flags & 0b00000110);
#else
    ASSERT(!(flags & 0b0001000001111100));
#endif

    const uiptr addr = (uiptr)base;
    idt_entry_t* const entry = &idt[index];

    entry->selector = selector;

#if EXOCORE_IS_32_BIT
    entry->base_low = addr & 0x0000ffff;
    entry->base_high = (addr >> 16) & 0x0000ffff;

    entry->flags = (ui8)flags;
#else
    entry->base_low = addr & 0x0000ffff;
    entry->base_middle = (addr >> 16) & 0x0000ffff;
    entry->base_high = addr >> 32;

    entry->flags = flags;
#endif

    entry->reserved = 0;
}

void initialize_idt(void)
{
    INFO("Initializing interrupt descriptor table... ");

    disable_interrupts();

    idt_pointer.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idt_pointer.base = &idt[0];

#if EXOCORE_IS_32_BIT
    const ui8 flags = 0b11101110;
#else
    const ui16 flags = 0b111011100000000; // TODO: We may want to use the IST later.
#endif

    idt_set_descriptor(ISR_CPU_START, &isr_0, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 1, &isr_1, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 2, &isr_2, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 3, &isr_3, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 4, &isr_4, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 5, &isr_5, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 6, &isr_6, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 7, &isr_7, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 8, &isr_8, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 9, &isr_9, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 10, &isr_10, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 11, &isr_11, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 12, &isr_12, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 13, &isr_13, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 14, &isr_14, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 15, &isr_15, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 16, &isr_16, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 17, &isr_17, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 18, &isr_18, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 19, &isr_19, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 20, &isr_20, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 21, &isr_21, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 22, &isr_22, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 23, &isr_23, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 24, &isr_24, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 25, &isr_25, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 26, &isr_26, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 27, &isr_27, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 28, &isr_28, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 29, &isr_29, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 30, &isr_30, 0x0008, flags);
    idt_set_descriptor(ISR_CPU_START + 31, &isr_31, 0x0008, flags);

    idt_set_descriptor(ISR_IRQ_MASTER_START, &irq_0, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 1, &irq_1, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 2, &irq_2, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 3, &irq_3, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 4, &irq_4, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 5, &irq_5, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 6, &irq_6, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_MASTER_START + 7, &irq_7, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START, &irq_8, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 1, &irq_9, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 2, &irq_10, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 3, &irq_11, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 4, &irq_12, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 5, &irq_13, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 6, &irq_14, 0x0008, flags);
    idt_set_descriptor(ISR_IRQ_SLAVE_START + 7, &irq_15, 0x0008, flags);

    idt_flush(&idt_pointer);

    enable_interrupts();

    SUCCESS("OK.\n");
}
