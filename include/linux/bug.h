#pragma once

#define BUG() \
    {         \
    }

#define WARN_ON(condition) ({  \
    int __ret = !!(condition); \
    __ret;                     \
})

#define BUG_ON(condition)        \
    do                           \
    {                            \
        if (unlikely(condition)) \
            BUG();               \
    } while (0)

#define WARN_ON_ONCE(condition) WARN_ON(condition)
