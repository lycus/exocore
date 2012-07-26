#include "i386/descriptors.h"

bool initialize_architecture(void)
{
    i386_initialize_descriptors();

    return true;
}
