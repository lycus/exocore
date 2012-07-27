#ifndef EXOCORE_H_ARCH
#define EXOCORE_H_ARCH

#include "exocore/common.h"

// Performs any architecture-specific initialization.
void initialize_architecture(void);

// Enables the receiving of interrupts from hardware.
void enable_interrupts(void);

// Disables the receiving of interrupts from hardware.
void disable_interrupts(void);

// Immediately halts the machine. This is irrecoverable.
attr(noreturn) void halt_machine(void);

#endif
