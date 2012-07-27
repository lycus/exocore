#include "exocore/console.h"
#include "exocore/interrupts.h"
#include "exocore/machine.h"

void panic(const char* const message)
{
    ASSERT(message);

    ERROR(message);

    disable_interrupts();
    halt_machine();
}
