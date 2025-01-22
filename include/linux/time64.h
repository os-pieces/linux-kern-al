#pragma once

#include <linux/types.h>

struct timespec64 {
	s64 tv_sec;
	s64 tv_nsec;
};

typedef s64 time64_t;
