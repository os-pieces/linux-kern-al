#pragma once

// TODO

typedef struct
{

} seqcount_spinlock_t;

typedef struct
{
	seqcount_spinlock_t seqcount;
} seqlock_t;

#define read_seqcount_begin(s) (0)
#define __read_seqcount_begin(s) (0)
#define read_seqcount_retry(s, start) (0)

#define write_seqcount_begin(s)
#define write_seqcount_end(s)

#define raw_seqcount_begin(s) (0)

#define DEFINE_SEQLOCK(sl) \
    seqlock_t sl = {}

static inline void read_seqlock_excl(seqlock_t *sl)
{
}

static inline void read_sequnlock_excl(seqlock_t *sl)
{
    //TODO
}

static inline unsigned read_seqbegin(const seqlock_t *sl)
{
    return 0;
}

static inline unsigned read_seqretry(const seqlock_t *sl, unsigned start)
{
	/*
	 * Assume not nested: read_seqretry() may be called multiple times when
	 * completing read critical section.
	 */

	return read_seqcount_retry(&sl->seqcount, start);
}
