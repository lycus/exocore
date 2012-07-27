#ifndef EXOCORE_H_TIMER
#define EXOCORE_H_TIMER

#include "exocore/common.h"

#define PIT_COMMAND_MODE 0x43
#define PIT_CHANNEL_0_DATA (PIT_COMMAND_MODE - 3)
#define PIT_CHANNEL_1_DATA (PIT_COMMAND_MODE - 2)
#define PIT_CHANNEL_2_DATA (PIT_COMMAND_MODE - 1)

#define TIMER_FREQUENCY 1000

// Initializes the programmable interrupt timer to keep the tick counter updated.
void initialize_pit(void);

// Gets the amount of ticks (milliseconds) since boot. Can overflow.
ui64 get_ticks();

#endif
