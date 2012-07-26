#include "exocore/arch.h"
#include "exocore/console.h"

void panic(const char* const message)
{
    disable_interrupts();

    ERROR(message);

    halt_machine();
}
