#pragma once

//TODO
typedef int atomic_t;

static inline void atomic_inc(atomic_t *v)
{
    (*v) ++;
}

static inline int atomic_inc_return(atomic_t *v)
{
    (*v) ++;

    return *v;
}
