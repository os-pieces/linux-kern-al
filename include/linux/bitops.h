#pragma once

#include <asm/ffs.h>

int constant_fls(unsigned int x);
#define fls(x) constant_fls(x)
