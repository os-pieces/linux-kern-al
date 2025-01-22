#pragma once

#include <linux/types.h>
#include <linux/gfp.h>
#include <linux/bug.h>
#include <linux/spinlock.h>

struct xarray
{
	spinlock_t xa_lock;
	/* private: The rest of the data structure is not to be used directly. */
	gfp_t xa_flags;
	void __rcu *xa_head;
};

/*
 * xa_is_internal() - Is the entry an internal entry?
 * @entry: XArray entry.
 *
 * Context: Any context.
 * Return: %true if the entry is an internal entry.
 */
static inline bool xa_is_internal(const void *entry)
{
	return ((uintptr_t)entry & 3) == 2;
}

/* Private */
static inline bool xa_is_node(const void *entry)
{
	return xa_is_internal(entry) && (uintptr_t)entry > 4096;
}

static inline int xa_err(void *entry)
{
	// TODO
	return 0;
}
