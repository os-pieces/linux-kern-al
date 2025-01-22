#pragma once

#include <linux/types.h>

#define GOLDEN_RATIO_32 0x61C88647
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

#define __hash_32 __hash_32_generic
static inline u32 __hash_32_generic(u32 val)
{
	return val * GOLDEN_RATIO_32;
}

static inline u32 hash_32(u32 val, unsigned int bits)
{
	/* High bits are more random, so use them. */
	return __hash_32(val) >> (32 - bits);
}

#define hash_64 hash_64_generic
static inline u32 hash_64_generic(u64 val, unsigned int bits)
{
    u32 ret;

    if (sizeof(uintptr_t) == 8)
    {
        /* 64x64-bit multiply is efficient on all 64-bit processors */
        ret = val * GOLDEN_RATIO_64 >> (64 - bits);
    }
	else
    {
        ret = hash_32((u32)val ^ __hash_32(val >> 32), bits);
    }

	return ret;
}

static inline u32 hash_long(uintptr_t val, unsigned int bits)
{
    u32 ret;

    if (sizeof(val) == 8)
    {
        ret = hash_64(val, bits);
    }
    else
    {
        ret = hash_32(val, bits);
    }

    return ret;
} 
