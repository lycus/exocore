#include "exocore/arch.h"
#include "exocore/console.h"

void panic(const char* const message)
{
    ASSERT(message);

    ERROR(message);

    disable_interrupts();
    halt_machine();
}
