#pragma once

#include <linux/types.h>
#include <linux/bitops.h>

static inline int __ilog2_u32(u32 n)
{
	return fls(n) - 1;
}

static inline bool is_power_of_2(unsigned long n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}

#define rounddown_pow_of_two(n) (1 << __ilog2_u32(n))
