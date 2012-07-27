#include "exocore/i386/descriptors.h"
#include "exocore/i386/pit.h"

void initialize_architecture(void)
{
    i386_initialize_descriptors();
    i386_initialize_timer(DEFAULT_FREQUENCY);
}
