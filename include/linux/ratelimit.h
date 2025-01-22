#pragma once

struct ratelimit_state
{
};

extern int ___ratelimit(struct ratelimit_state *rs, const char *func);

#define __ratelimit(state) ___ratelimit(state, __func__)

#define DEFAULT_RATELIMIT_INTERVAL	(5)
#define DEFAULT_RATELIMIT_BURST		10

#define ratelimit_state_init(...)
