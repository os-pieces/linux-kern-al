#pragma once

#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/gfp.h>
#include <linux/slab_def.h>

#define SLAB_ACCOUNT		0
#define SLAB_RECLAIM_ACCOUNT	((slab_flags_t __force)0)

void *__kmalloc(size_t size, gfp_t flags);
void kfree(const void *objp);

static inline void *kmalloc_array(size_t n, size_t size, gfp_t flags)
{
    size_t bytes;

    bytes = n * size;

    return __kmalloc(bytes, flags);
}

static inline void *kcalloc(size_t n, size_t size, gfp_t flags)
{
	return kmalloc_array(n, size, flags | __GFP_ZERO);
}

static inline void *kzalloc(size_t size, gfp_t flags)
{
    return __kmalloc(size, flags | __GFP_ZERO);
}

static inline void *kmalloc(size_t size, gfp_t flags)
{
    return __kmalloc(size, flags);
}

struct kmem_cache *kmem_cache_create(const char *name, unsigned int size,
			unsigned int align, slab_flags_t flags,
			void (*ctor)(void *));
void kmem_cache_destroy(struct kmem_cache *s);
void *kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags);
void kmem_cache_free(struct kmem_cache *s, void *objp);
