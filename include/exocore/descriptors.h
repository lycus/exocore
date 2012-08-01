#ifndef EXOCORE_H_DESCRIPTORS
#define EXOCORE_H_DESCRIPTORS

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

#define MAX_DPL 3

#endif
