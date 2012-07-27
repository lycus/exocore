#include "exocore/console.h"
#include "exocore/i386/interrupts.h"
#include "exocore/i386/io.h"
#include "exocore/i386/pit.h"

static ui64 current_ticks;

void i386_initialize_timer(const ui32 frequency)
{
    INFO("Initializing i386 interrupt timer... ");

    // Firstly, register our timer callback.
    //register_interrupt_handler(IRQ0, &timer_callback);
    set_interrupt_handler(INTERRUPT_IRQ_TIMER, ^(const interrupt_info_t info attr(unused))
    {
        current_ticks++;
    });

    // The PIT operates at 315/265 MHz. The value we send it is the value
    // it should divide its input clock (approximately 1193182 Hz) by. The
    // value we send must fit in 16 bits.
    const ui32 divisor = 1193182 / frequency;

    // Send the command byte.
    i386_io_write_ui8(PIT_COMMAND_MODE, 0x36);
    i386_io_wait();

    // Divisor has to be sent byte-wise, so split it into upper/lower bytes.
    const ui8 low = divisor & 0xff;
    const ui8 high = (divisor >> 8) & 0xff;

    // Send the frequency divisor.
    i386_io_write_ui8(PIT_CHANNEL_0_DATA, low);
    i386_io_wait();
    i386_io_write_ui8(PIT_CHANNEL_0_DATA, high);
    i386_io_wait();

    SUCCESS("OK.\n");
}

ui64 get_ticks()
{
    return current_ticks;
}
