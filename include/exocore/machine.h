#ifndef EXOCORE_H_MACHINE
#define EXOCORE_H_MACHINE

// Immediately halts the machine. This is irrecoverable.
attr(noreturn) void halt_machine(void);

void breakpoint(void);

#endif
