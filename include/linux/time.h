#pragma once

#include <linux/time64.h>
#include <linux/time_types.h>

void time64_to_tm(time64_t totalsecs, int offset, struct tm *result);

#define HZ 1000
