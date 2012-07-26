#include "console.h"

void panic(const char* const message)
{
    ERROR(message);

    for (;;)
    {
    }
}
