#pragma once

#include <linux/kernel.h>
#include <linux/rcupdate.h>
#include <linux/spinlock.h>

#define MAPLE_NODE_SLOTS 63 /* 256 bytes including ->parent */
#define MAPLE_ARANGE64_SLOTS	21	/* 240 bytes */
#define MAPLE_RANGE64_SLOTS	32	/* 256 bytes */

#define MAPLE_NODE_MASK 255UL

#define MAPLE_RESERVED_RANGE	4096

#define MAPLE_NODE_TYPE_MASK	0x0F
#define MAPLE_NODE_TYPE_SHIFT	0x03

#define MT_FLAGS_LOCK_MASK	0x300
#define MT_FLAGS_LOCK_EXTERN	0x300
#define MT_FLAGS_ALLOC_WRAPPED	0x0800
#define MT_FLAGS_ALLOC_RANGE	0x01

#define mtree_lock(mt)		spin_lock((&(mt)->ma_lock))
#define mtree_unlock(mt)	spin_unlock((&(mt)->ma_lock))

enum maple_type
{
	maple_dense,
	maple_leaf_64,
	maple_range_64,
	maple_arange_64,
};

enum maple_status
{
	ma_active,
	ma_start,
	ma_root,
	ma_none,
	ma_pause,
	ma_overflow,
	ma_underflow,
	ma_error,
};

struct maple_arange_64
{
	uintptr_t pivot[MAPLE_ARANGE64_SLOTS - 1];
	void __rcu *slot[MAPLE_ARANGE64_SLOTS];
};

struct maple_range_64
{
	struct maple_pnode *parent;
	uintptr_t pivot[MAPLE_RANGE64_SLOTS - 1];
	union
	{
		void __rcu *slot[MAPLE_RANGE64_SLOTS];
	};
};

struct maple_tree
{
	union
	{
		spinlock_t ma_lock;
	};
	unsigned int ma_flags;
	void *ma_root;
};

struct ma_state
{
	struct maple_tree *tree; /* The tree we're operating in */
	enum maple_status status;
	uintptr_t min;			  /* The minimum index of this node - implied pivot min */
	uintptr_t max;			  /* The maximum index of this node - implied pivot max */
	uintptr_t index;		  /* The index we're operating on - range start */
	uintptr_t last;			  /* The last index we're operating on - range end */
	struct maple_enode *node; /* The node containing this entry */
	unsigned char depth;	  /* depth of tree descent during write */
	unsigned char offset;
	unsigned char mas_flags;
	unsigned char end; /* The end of the node */
};

struct maple_node
{
	union
	{
		struct
		{
			struct maple_pnode *parent;
			void *slot[MAPLE_NODE_SLOTS];
		};

		struct maple_range_64 mr64;
		struct maple_arange_64 ma64;
	};
};

#define MA_STATE(name, mt, first, end) \
	struct ma_state name = {}

void *mas_find(struct ma_state *mas, unsigned long max);
int mtree_alloc_cyclic(struct maple_tree *mt, uintptr_t *startp,
		void *entry, uintptr_t range_lo, uintptr_t range_hi,
		uintptr_t *next, gfp_t gfp);
void mt_init_flags(struct maple_tree *mt, unsigned int flags);

static inline bool mt_external_lock(const struct maple_tree *mt)
{
	return (mt->ma_flags & MT_FLAGS_LOCK_MASK) == MT_FLAGS_LOCK_EXTERN;
}
