#ifndef EXOCORE_H_I386_PIT
#define EXOCORE_H_I386_PIT

#define PIT_COMMAND_MODE 0x43
#define PIT_CHANNEL_0_DATA (PIT_COMMAND_MODE - 3)
#define PIT_CHANNEL_1_DATA (PIT_COMMAND_MODE - 2)
#define PIT_CHANNEL_2_DATA (PIT_COMMAND_MODE - 1)

#define DEFAULT_FREQUENCY 1000

void i386_initialize_timer(const ui32 frequency);

#endif
