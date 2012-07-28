#ifndef EXOCORE_H_TIMER
#define EXOCORE_H_TIMER

#include "exocore/common.h"

#define TIMER_FREQUENCY 1000

// Initializes the programmable interrupt timer to keep the tick counter updated.
void initialize_pit(void);

// Gets the amount of ticks (milliseconds) since boot. Can overflow.
ui64 get_ticks();

#endif
