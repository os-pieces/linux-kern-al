#pragma once

#include <linux/list.h>

#define hlist_first_rcu(head)	(*((struct hlist_node __rcu **)(&(head)->first)))

static inline void hlist_add_head_rcu(struct hlist_node *n, struct hlist_head *h)
{
	struct hlist_node *first = h->first;

	n->next = first;
	WRITE_ONCE(n->pprev, &h->first);
	rcu_assign_pointer(hlist_first_rcu(h), n);
	if (first)
		WRITE_ONCE(first->pprev, &n->next);
}

