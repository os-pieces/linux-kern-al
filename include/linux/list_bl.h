#pragma once

#include <linux/bit_spinlock.h>

#define LIST_BL_LOCKMASK	0

struct hlist_bl_head {
	struct hlist_bl_node *first;
};

struct hlist_bl_node {
	struct hlist_bl_node *next, **pprev;
};

#define hlist_bl_entry(ptr, type, member) container_of(ptr,type,member)

static inline void INIT_HLIST_BL_NODE(struct hlist_bl_node *h)
{
	h->next = NULL;
	h->pprev = NULL;
}

static inline bool hlist_bl_is_locked(struct hlist_bl_head *b)
{
	return bit_spin_is_locked(0, (unsigned long *)b);
}

static inline void hlist_bl_lock(struct hlist_bl_head *b)
{
	pr_todo();
}

static inline void hlist_bl_unlock(struct hlist_bl_head *b)
{

}

static inline struct hlist_bl_node *hlist_bl_first(struct hlist_bl_head *h)
{
	return (struct hlist_bl_node *)
        ((uintptr_t)h->first & ~LIST_BL_LOCKMASK);
}

static inline bool  hlist_bl_unhashed(const struct hlist_bl_node *h)
{
	return !h->pprev;
}
