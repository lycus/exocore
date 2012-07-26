#include "arch.h"
#include "console.h"

void panic(const char* const message)
{
    disable_interrupts();

    ERROR(message);

    halt_machine();
}
