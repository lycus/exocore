#ifndef EXOCORE_H_IO
#define EXOCORE_H_IO

ui8 io_read_ui8(ui16 port);
ui16 io_read_ui16(ui16 port);
ui32 io_read_ui32(ui16 port);

void io_write_ui8(ui16 port, ui8 value);
void io_write_ui16(ui16 port, ui16 value);
void io_write_ui32(ui16 port, ui32 value);

// Waits for an I/O operation to complete.
void io_wait(void);

#endif
