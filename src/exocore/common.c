#include "console.h"

void panic(const char* const message)
{
    console_print(CONSOLE_COLOR_LIGHT_RED, CONSOLE_COLOR_BLACK, message);

    for (;;)
    {
    }
}
