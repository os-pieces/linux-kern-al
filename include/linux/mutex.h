#pragma once

struct mutex
{
};

#define mutex_init(mutex)
extern void mutex_lock(struct mutex *lock);
extern void mutex_unlock(struct mutex *lock);
