#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#ifndef __bitwise
#define __bitwise
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;
typedef uint32_t u32;
typedef unsigned short umode_t;
typedef long long loff_t;
typedef unsigned int gfp_t;
typedef unsigned int __bitwise fmode_t;

typedef uint64_t __u64;

struct list_head {
	struct list_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};

typedef uint16_t __bitwise __le16;
typedef uint32_t __bitwise __le32;
typedef uint8_t __u8;
typedef uint32_t __u32;

/**
 * The type used for indexing onto a disc or disc partition.
 *
 * Linux always considers sectors to be 512 bytes long independently
 * of the devices real block size.
 *
 * blkcnt_t is the type of the inode's block count.
 */
typedef u64 sector_t;
typedef u64 blkcnt_t;

typedef uint16_t uid_t;
typedef uint16_t gid_t;

typedef int64_t s64;
typedef int32_t s32;

struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *head);
} __attribute__((aligned(sizeof(void *))));
#define rcu_head callback_head

typedef unsigned int __bitwise slab_flags_t;

//typedef unsigned long ino_t;
