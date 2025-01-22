#pragma once

#define MAX_ERRNO 4095

#define IS_ERR_VALUE(x) ((unsigned long long)(x) >= (unsigned long long)-MAX_ERRNO)

static inline void* ERR_PTR(long long error)
{
    return (void *)error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long long)ptr;
}

static inline bool IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE(ptr);
}

static inline bool IS_ERR_OR_NULL(const void *ptr)
{
    return (!ptr) || IS_ERR_VALUE(ptr);
}

/**
 * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
 * @ptr: The pointer to cast.
 *
 * Explicitly cast an error-valued pointer to another pointer type in such a
 * way as to make it clear that's what's going on.
 */
static inline void* ERR_CAST(const void *ptr)
{
    /* cast away the const */
    return (void *)ptr;
}

static inline int PTR_ERR_OR_ZERO(const void *ptr)
{
    if (IS_ERR(ptr))
        return PTR_ERR(ptr);
    else
        return 0;
}
