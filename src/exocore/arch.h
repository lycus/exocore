#ifndef EXOCORE_H_ARCH
#define EXOCORE_H_ARCH

#include "common.h"

// Performs any architecture-specific initialization.
bool initialize_architecture(void);

ui8 io_read_ui8(ui16 port);
ui16 io_read_ui16(ui16 port);
ui32 io_read_ui32(ui16 port);

void io_write_ui8(ui16 port, ui8 value);
void io_write_ui16(ui16 port, ui16 value);
void io_write_ui32(ui16 port, ui32 value);

void enable_interrupts(void);
void disable_interrupts(void);

#endif
