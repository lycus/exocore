#include "i386/descriptors.h"

bool architecture_initialize(void)
{
    i386_initialize_descriptors();

    return true;
}
