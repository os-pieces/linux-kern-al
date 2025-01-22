#include <linux/slab.h>
#include <linux/printk.h>

#include <stdlib.h>
#include <string.h>

void *__kmalloc(size_t size, gfp_t flags)
{
    void *p;

    p = malloc(size);
    if (p && (flags & __GFP_ZERO))
    {
        memset(p, 0, size);
    }

    return p;
}

void kfree(const void *p)
{
    free((void*)p);
}

struct kmem_cache *kmem_cache_create(const char *name, unsigned int size,
			unsigned int align, slab_flags_t flags,
			void (*ctor)(void *))
{
    struct kmem_cache *km;

    km = malloc(sizeof(struct kmem_cache));

    return km;
}

void kmem_cache_destroy(struct kmem_cache *s)
{

}

void *kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags)
{
    return NULL;
}

void kmem_cache_free(struct kmem_cache *s, void *objp)
{

}
