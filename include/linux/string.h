#pragma once

#include <linux/types.h>	/* for size_t */
#include <string.h>

extern char *kmemdup_nul(const char *s, size_t len, gfp_t gfp);
extern void *kmemdup(const void *src, size_t len, gfp_t gfp);
