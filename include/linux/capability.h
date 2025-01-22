#pragma once

#include <linux/types.h>

#define CAP_LINUX_IMMUTABLE  9

#define CAP_SYS_ADMIN  12

static inline bool capable(int cap)
{
	return true;
}
