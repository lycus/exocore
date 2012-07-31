#ifndef EXOCORE_H_MUTEX
#define EXOCORE_H_MUTEX

typedef struct mutex
{
    bool locked; // Zero if unlocked; otherwise, non-zero.
} mutex_t;

void mutex_acquire(mutex_t* const mutex);

void mutex_release(mutex_t* const mutex);

bool mutex_locked(const mutex_t* const mutex);

#endif
