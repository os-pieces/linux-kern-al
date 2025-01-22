#pragma once

#define unlikely(x) (x)
#define likely(x) (x)
#define __user
#define __force
#define __kernel
#define __rcu

#define WRITE_ONCE(x, val) (x = val)
#define READ_ONCE(x) (x)


#define __UNIQUE_ID(prefix) (prefix)

#define __cold
#define __printf(a, b)
