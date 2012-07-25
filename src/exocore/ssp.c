#include "common.h"

uiptr __stack_chk_guard = 0;

void __stack_chk_guard_setup()
{
    ui8* p = (ui8*)&__stack_chk_guard;

    p[sizeof(__stack_chk_guard) - 1] = 0xff;
    p[sizeof(__stack_chk_guard) - 2] = 0x0a;
}

void attr(noreturn) __stack_chk_fail()
{
    // TODO: Panic here.

    for (;;)
    {
    }
}

void ssp_setup()
{
    __stack_chk_guard_setup();
}
