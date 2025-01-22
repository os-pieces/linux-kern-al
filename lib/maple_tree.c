#include <linux/maple_tree.h>
#include <linux/xarray.h>
#include <linux/slab.h>
#include <linux/errno.h>

#define MA_ROOT_PARENT 1

/* Bit 1 indicates the root is a node */
#define MAPLE_ROOT_NODE 0x02
/* maple_type stored bit 3-6 */
#define MAPLE_ENODE_TYPE_SHIFT 0x03
/* Bit 2 means a NULL somewhere below */
#define MAPLE_ENODE_NULL 0x04

static const unsigned char mt_pivots[] = {
    [maple_dense] = 0,
    [maple_leaf_64] = MAPLE_RANGE64_SLOTS - 1,
    [maple_range_64] = MAPLE_RANGE64_SLOTS - 1,
    [maple_arange_64] = MAPLE_ARANGE64_SLOTS - 1,
};

static inline void *mte_safe_root(const struct maple_enode *node)
{
    return (void *)((uintptr_t)node & ~MAPLE_ROOT_NODE);
}

static inline bool mas_is_start(const struct ma_state *mas)
{
    return mas->status == ma_start;
}

static inline bool mas_is_ptr(const struct ma_state *mas)
{
    return mas->status == ma_root;
}

static inline bool mas_is_none(const struct ma_state *mas)
{
    return mas->status == ma_none;
}

static inline bool mas_is_active(struct ma_state *mas)
{
    return mas->status == ma_active;
}

static inline bool mas_is_err(struct ma_state *mas)
{
    return mas->status == ma_error;
}

static inline bool ma_is_root(struct maple_node *node)
{
    return ((uintptr_t)node->parent & MA_ROOT_PARENT);
}

static inline bool mas_is_overflow(struct ma_state *mas)
{
    return mas->status == ma_overflow;
}

static inline bool mt_is_alloc(struct maple_tree *mt)
{
    return (mt->ma_flags & MT_FLAGS_ALLOC_RANGE);
}

static inline bool mt_is_reserved(const void *entry)
{
    return ((uintptr_t)entry < MAPLE_RESERVED_RANGE) && xa_is_internal(entry);
}

static inline void *mas_root(struct ma_state *mas)
{
    return mas->tree->ma_root;
}

static inline void *mt_slot(const struct maple_tree *mt, void __rcu **slots, unsigned char offset)
{
    return slots[offset]; // TODO
}

static inline struct maple_node *mte_to_node(const struct maple_enode *entry)
{
    return (struct maple_node *)((uintptr_t)entry & ~MAPLE_NODE_MASK);
}

/*
 * mas_mn() - Get the maple state node.
 * @mas: The maple state
 *
 * Return: the maple node (not encoded - bare pointer).
 */
static inline struct maple_node *mas_mn(const struct ma_state *mas)
{
    return mte_to_node(mas->node);
}

static inline uintptr_t mas_safe_pivot(const struct ma_state *mas, uintptr_t *pivots,
                                       unsigned char piv, enum maple_type type)
{
    if (piv >= mt_pivots[type])
        return mas->max;

    return pivots[piv];
}

/*
 * mas_slot() - Get the slot value when not holding the maple tree lock.
 * @mas: The maple state
 * @slots: The pointer to the slots
 * @offset: The offset into the slots array to fetch
 *
 * Return: The entry stored in @slots at the @offset
 */
static inline void *mas_slot(struct ma_state *mas, void __rcu **slots,
                             unsigned char offset)
{
    return mt_slot(mas->tree, slots, offset);
}

static inline enum maple_type mte_node_type(const struct maple_enode *entry)
{
    return ((uintptr_t)entry >> MAPLE_NODE_TYPE_SHIFT) & MAPLE_NODE_TYPE_MASK;
}

/*
 * mte_parent() - Get the parent of @node.
 * @enode: The encoded maple node.
 *
 * Return: The parent maple node.
 */
static inline struct maple_node *mte_parent(const struct maple_enode *enode)
{
    return (void *)((uintptr_t)(mte_to_node(enode)->parent) & ~MAPLE_NODE_MASK);
}

static void *mtree_range_walk(struct ma_state *mas)
{
    // TODO
    return NULL;
}

static inline unsigned char ma_data_end(struct maple_node *node,
                                        enum maple_type type, uintptr_t *pivots, uintptr_t max)
{
    // TODO
    return 0;
}

static int mas_ascend(struct ma_state *mas)
{
    // TODO
    return 0;
}

/*
 * mte_dead_node() - check if the @enode is dead.
 * @enode: The encoded maple node
 *
 * Return: true if dead, false otherwise.
 */
static bool mte_dead_node(const struct maple_enode *enode)
{
    struct maple_node *parent, *node;

    node = mte_to_node(enode);

    parent = mte_parent(enode);

    return (parent == node);
}

static struct maple_enode *mas_start(struct ma_state *mas)
{
    if (likely(mas_is_start(mas)))
    {
        struct maple_enode *root;

        mas->min = 0;
        mas->max = ULONG_MAX;

    retry:
        mas->depth = 0;
        root = mas_root(mas);
        /* Tree with nodes */
        if (likely(xa_is_node(root)))
        {
            mas->depth = 1;
            mas->status = ma_active;
            mas->node = mte_safe_root(root);
            mas->offset = 0;
            if (mte_dead_node(mas->node))
                goto retry;

            return NULL;
        }

        mas->node = NULL;
        /* empty tree */
        if (unlikely(!root))
        {
            mas->status = ma_none;
            mas->offset = MAPLE_NODE_SLOTS;
            return NULL;
        }

        /* Single entry tree */
        mas->status = ma_root;
        mas->offset = MAPLE_NODE_SLOTS;

        /* Single entry tree. */
        if (mas->index > 0)
            return NULL;

        return root;
    }

    return NULL;
}

static void *mas_state_walk(struct ma_state *mas)
{
    void *entry;

    entry = mas_start(mas);
    if (mas_is_none(mas))
        return NULL;

    if (mas_is_ptr(mas))
        return entry;

    return mtree_range_walk(mas);
}

/**
 * mas_walk() - Search for @mas->index in the tree.
 * @mas: The maple state.
 *
 * mas->index and mas->last will be set to the range if there is a value.  If
 * mas->status is ma_none, reset to ma_start
 *
 * Return: the entry at the location or %NULL.
 */
void *mas_walk(struct ma_state *mas)
{
    void *entry;

    if (!mas_is_active(mas) || !mas_is_start(mas))
        mas->status = ma_start;
retry:
    entry = mas_state_walk(mas);
    if (mas_is_start(mas))
    {
        goto retry;
    }
    else if (mas_is_none(mas))
    {
        mas->index = 0;
        mas->last = ULONG_MAX;
    }
    else if (mas_is_ptr(mas))
    {
        if (!mas->index)
        {
            mas->last = 0;
            return entry;
        }

        mas->index = 1;
        mas->last = ULONG_MAX;
        mas->status = ma_none;
        return NULL;
    }

    return entry;
}

static bool mas_find_setup(struct ma_state *mas, uintptr_t max, void **entry)
{
    switch (mas->status)
    {
    case ma_active:
        if (mas->last < max)
            return false;
        return true;
    case ma_start:
        break;
    case ma_pause:
        if (unlikely(mas->last >= max))
            return true;

        mas->index = ++mas->last;
        mas->status = ma_start;
        break;
    case ma_none:
        if (unlikely(mas->last >= max))
            return true;

        mas->index = mas->last;
        mas->status = ma_start;
        break;
    case ma_underflow:
        /* mas is pointing at entry before unable to go lower */
        if (unlikely(mas->index >= max))
        {
            mas->status = ma_overflow;
            return true;
        }

        mas->status = ma_active;
        *entry = mas_walk(mas);
        if (*entry)
            return true;
        break;
    case ma_overflow:
        if (unlikely(mas->last >= max))
            return true;

        mas->status = ma_active;
        *entry = mas_walk(mas);
        if (*entry)
            return true;
        break;
    case ma_root:
        break;
    case ma_error:
        return true;
    }

    if (mas_is_start(mas))
    {
        /* First run or continue */
        if (mas->index > max)
            return true;

        *entry = mas_walk(mas);
        if (*entry)
            return true;
    }

    if (unlikely(mas_is_ptr(mas)))
        goto ptr_out_of_range;

    if (unlikely(mas_is_none(mas)))
        return true;

    if (mas->index == max)
        return true;

    return false;

ptr_out_of_range:
    mas->status = ma_none;
    mas->index = 1;
    mas->last = ULONG_MAX;
    return true;
}

static inline void mas_reset(struct ma_state *mas)
{
    mas->status = ma_start;
    mas->node = NULL;
}

/*
 * ma_dead_node() - check if the @enode is dead.
 * @enode: The encoded maple node
 *
 * Return: true if dead, false otherwise.
 */
static inline bool ma_dead_node(const struct maple_node *node)
{
    struct maple_node *parent;

    /* Do not reorder reads from the node prior to the parent check */
    smp_rmb();
    parent = (void *)((uintptr_t)node->parent & ~MAPLE_NODE_MASK);

    return (parent == node);
}

static inline void mas_set(struct ma_state *mas, unsigned long index)
{
    // TODO
}

static void mas_rewalk(struct ma_state *mas, uintptr_t index)
{
retry:
    mas_set(mas, index);
    mas_state_walk(mas);
    if (mas_is_start(mas))
        goto retry;
}

static inline bool mas_rewalk_if_dead(struct ma_state *mas,
                                      struct maple_node *node, const uintptr_t index)
{
    if (unlikely(ma_dead_node(node)))
    {
        mas_rewalk(mas, index);
        return true;
    }

    return false;
}

static inline unsigned long *ma_pivots(struct maple_node *node,
                                       enum maple_type type)
{
    switch (type)
    {
    case maple_arange_64:
        return node->ma64.pivot;
    case maple_range_64:
    case maple_leaf_64:
        return node->mr64.pivot;
    case maple_dense:
        return NULL;
    }
    return NULL;
}

static inline void __rcu **ma_slots(struct maple_node *mn, enum maple_type mt)
{
    switch (mt)
    {
    case maple_arange_64:
        return mn->ma64.slot;
    case maple_range_64:
    case maple_leaf_64:
        return mn->mr64.slot;
    case maple_dense:
        return mn->slot;
    }

    return NULL;
}

static int mas_next_node(struct ma_state *mas, struct maple_node *node,
                         uintptr_t max)
{
    uintptr_t min;
    uintptr_t *pivots;
    struct maple_enode *enode;
    struct maple_node *tmp;
    int level = 0;
    unsigned char node_end;
    enum maple_type mt;
    void __rcu **slots;

    if (mas->max >= max)
        goto overflow;

    min = mas->max + 1;
    level = 0;
    do
    {
        if (ma_is_root(node))
            goto overflow;

        /* Walk up. */
        if (unlikely(mas_ascend(mas)))
            return 1;

        level++;
        node = mas_mn(mas);
        mt = mte_node_type(mas->node);
        pivots = ma_pivots(node, mt);
        node_end = ma_data_end(node, mt, pivots, mas->max);
        if (unlikely(ma_dead_node(node)))
            return 1;

    } while (unlikely(mas->offset == node_end));

    slots = ma_slots(node, mt);
    mas->offset++;
    enode = mas_slot(mas, slots, mas->offset);
    if (unlikely(ma_dead_node(node)))
        return 1;

    if (level > 1)
        mas->offset = 0;

    while (unlikely(level > 1))
    {
        level--;
        mas->node = enode;
        node = mas_mn(mas);
        mt = mte_node_type(mas->node);
        slots = ma_slots(node, mt);
        enode = mas_slot(mas, slots, 0);
        if (unlikely(ma_dead_node(node)))
            return 1;
    }

    if (!mas->offset)
        pivots = ma_pivots(node, mt);

    mas->max = mas_safe_pivot(mas, pivots, mas->offset, mt);
    tmp = mte_to_node(enode);
    mt = mte_node_type(enode);
    pivots = ma_pivots(tmp, mt);
    mas->end = ma_data_end(tmp, mt, pivots, mas->max);
    if (unlikely(ma_dead_node(node)))
        return 1;

    mas->node = enode;
    mas->min = min;
    return 0;

overflow:
    if (unlikely(ma_dead_node(node)))
        return 1;

    mas->status = ma_overflow;
    return 0;
}

/*
 * mas_next_slot() - Get the entry in the next slot
 *
 * @mas: The maple state
 * @max: The maximum starting range
 * @empty: Can be empty
 *
 * Return: The entry in the next slot which is possibly NULL
 */
static void *mas_next_slot(struct ma_state *mas, unsigned long max, bool empty)
{
    void __rcu **slots;
    uintptr_t *pivots;
    uintptr_t pivot;
    enum maple_type type;
    struct maple_node *node;
    uintptr_t save_point = mas->last;
    void *entry;

retry:
    node = mas_mn(mas);
    type = mte_node_type(mas->node);
    pivots = ma_pivots(node, type);
    if (unlikely(mas_rewalk_if_dead(mas, node, save_point)))
        goto retry;

    if (mas->max >= max)
    {
        if (likely(mas->offset < mas->end))
            pivot = pivots[mas->offset];
        else
            pivot = mas->max;

        if (unlikely(mas_rewalk_if_dead(mas, node, save_point)))
            goto retry;

        if (pivot >= max)
        { /* Was at the limit, next will extend beyond */
            mas->status = ma_overflow;
            return NULL;
        }
    }

    if (likely(mas->offset < mas->end))
    {
        mas->index = pivots[mas->offset] + 1;
    again:
        mas->offset++;
        if (likely(mas->offset < mas->end))
            mas->last = pivots[mas->offset];
        else
            mas->last = mas->max;
    }
    else
    {
        if (mas->last >= max)
        {
            mas->status = ma_overflow;
            return NULL;
        }

        if (mas_next_node(mas, node, max))
        {
            mas_rewalk(mas, save_point);
            goto retry;
        }

        if (WARN_ON_ONCE(mas_is_overflow(mas)))
            return NULL;

        mas->offset = 0;
        mas->index = mas->min;
        node = mas_mn(mas);
        type = mte_node_type(mas->node);
        pivots = ma_pivots(node, type);
        mas->last = pivots[0];
    }

    slots = ma_slots(node, type);
    entry = mt_slot(mas->tree, slots, mas->offset);
    if (unlikely(mas_rewalk_if_dead(mas, node, save_point)))
        goto retry;

    if (entry)
        return entry;

    if (!empty)
    {
        if (mas->last >= max)
        {
            mas->status = ma_overflow;
            return NULL;
        }

        mas->index = mas->last + 1;
        goto again;
    }

    return entry;
}

void mas_destroy(struct ma_state *mas)
{
    //TODO
}

bool mas_nomem(struct ma_state *mas, gfp_t gfp)
{
    //TODO
    return false;
}

void *mas_find(struct ma_state *mas, unsigned long max)
{
    void *entry = NULL;

    if (mas_find_setup(mas, max, &entry))
        return entry;

    /* Retries on dead nodes handled by mas_next_slot */
    entry = mas_next_slot(mas, max, false);
    /* Ignore overflow */
    mas->status = ma_active;

    return entry;
}

int mas_empty_area(struct ma_state *mas, unsigned long min,
		unsigned long max, unsigned long size)
{
    //TODO
    return 0;
}

static inline void *mas_insert(struct ma_state *mas, void *entry)
{
    //TODO
    return entry;
}

int mas_alloc_cyclic(struct ma_state *mas, unsigned long *startp,
                     void *entry, unsigned long range_lo, unsigned long range_hi,
                     unsigned long *next, gfp_t gfp)
{
    unsigned long min = range_lo;
    int ret = 0;

    range_lo = max(min, *next);
    ret = mas_empty_area(mas, range_lo, range_hi, 1);
    if ((mas->tree->ma_flags & MT_FLAGS_ALLOC_WRAPPED) && ret == 0)
    {
        mas->tree->ma_flags &= ~MT_FLAGS_ALLOC_WRAPPED;
        ret = 1;
    }
    if (ret < 0 && range_lo > min)
    {
        ret = mas_empty_area(mas, min, range_hi, 1);
        if (ret == 0)
            ret = 1;
    }
    if (ret < 0)
        return ret;

    do
    {
        mas_insert(mas, entry);
    } while (mas_nomem(mas, gfp));
    if (mas_is_err(mas))
        return xa_err(mas->node);

    *startp = mas->index;
    *next = *startp + 1;
    if (*next == 0)
        mas->tree->ma_flags |= MT_FLAGS_ALLOC_WRAPPED;

    mas_destroy(mas);
    return ret;
}

int mtree_alloc_cyclic(struct maple_tree *mt, uintptr_t *startp,
                       void *entry, uintptr_t range_lo, uintptr_t range_hi,
                       uintptr_t *next, gfp_t gfp)
{
    int ret;

    MA_STATE(mas, mt, 0, 0);

    if (!mt_is_alloc(mt))
        return -EINVAL;
    if (WARN_ON_ONCE(mt_is_reserved(entry)))
        return -EINVAL;

    mtree_lock(mt);
    ret = mas_alloc_cyclic(&mas, startp, entry, range_lo, range_hi,
                           next, gfp);
    mtree_unlock(mt);

    return ret;
}

void mt_init_flags(struct maple_tree *mt, unsigned int flags)
{
    mt->ma_flags = flags;
    if (!mt_external_lock(mt))
        spin_lock_init(&mt->ma_lock);
    rcu_assign_pointer(mt->ma_root, NULL);
}
