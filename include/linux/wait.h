#pragma once

#include <linux/spinlock.h>

typedef struct wait_queue_head
{

} wait_queue_head_t;

# define DECLARE_WAIT_QUEUE_HEAD_ONSTACK(name) \
	struct wait_queue_head name
