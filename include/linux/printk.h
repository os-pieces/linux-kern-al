#pragma once

#include <stdarg.h>
#include <stdio.h>

#include <linux/kern_levels.h>

#define printk printf

#define pr_todo() printk("TODO:%s:%d\n", __func__, __LINE__)

#define _printk printf

struct va_format {
	const char *fmt;
	va_list *va;
};

#define printk_index_subsys_emit(subsys_fmt_prefix, level, fmt, ...)
