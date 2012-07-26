#ifndef EXOCORE_H_I386_IO
#define EXOCORE_H_I386_IO

ui8 i386_io_read_ui8(ui16 port);
ui16 i386_io_read_ui16(ui16 port);
ui32 i386_io_read_ui32(ui16 port);

void i386_io_write_ui8(ui16 port, ui8 value);
void i386_io_write_ui16(ui16 port, ui16 value);
void i386_io_write_ui32(ui16 port, ui32 value);

// Waits for an I/O operation to complete.
void i386_io_wait(void);

#endif
