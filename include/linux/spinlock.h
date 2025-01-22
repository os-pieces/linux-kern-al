#pragma once

#include <asm/barrier.h>

typedef struct {

} spinlock_t;

#define spin_lock(...)
#define spin_unlock(...)

#define DEFINE_SPINLOCK(x)	spinlock_t x
#define spin_lock_init(x)
