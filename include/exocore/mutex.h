#ifndef EXOCORE_H_MUTEX
#define EXOCORE_H_MUTEX

typedef struct mutex
{
    bool locked; // Zero if unlocked; otherwise, non-zero.
} mutex_t;

void mutex_acquire(mutex_t* mutex);

void mutex_release(mutex_t* mutex);

bool mutex_locked(mutex_t* mutex);

#endif
