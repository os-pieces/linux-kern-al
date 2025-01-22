#pragma once

#include <linux/kernel.h>

#define __put_user(x, ptr) \
    ({ \
        *(ptr) = (x); \
        0; \
    })

static unsigned long raw_copy_from_user(void *to, const void __user *from, unsigned long n)
{
    memcpy(to, (const void __force *)from, n);

    return 0;
}
