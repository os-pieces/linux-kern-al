#pragma once

//TODO
#define rcu_read_lock()
#define rcu_read_unlock()

static inline int rcu_read_lock_held(void)
{
	return 1;
}

#define __rcu_dereference_raw(p, local) \
({ \
	/* Dependency order vs. p above. */ \
	typeof(p) local = READ_ONCE(p); \
	((typeof(*p) __force __kernel *)(local)); \
})
#define rcu_dereference_raw(p) __rcu_dereference_raw(p, __UNIQUE_ID(rcu))

#define __rcu_dereference_check(p, local, c, space) \
({ \
	/* Dependency order vs. p above. */ \
	typeof(*p) *local = (typeof(*p) *__force)READ_ONCE(p); \
	((typeof(*p) __force __kernel *)(local)); \
})

/**
 * rcu_dereference_check() - rcu_dereference with debug checking
 * @p: The pointer to read, prior to dereferencing
 * @c: The conditions under which the dereference will take place
 *
 * Do an rcu_dereference(), but check that the conditions under which the
 * dereference will take place are correct.  Typically the conditions
 * indicate the various locking conditions that should be held at that
 * point.  The check should return true if the conditions are satisfied.
 * An implicit check for being in an RCU read-side critical section
 * (rcu_read_lock()) is included.
 *
 * For example:
 *
 *	bar = rcu_dereference_check(foo->bar, lockdep_is_held(&foo->lock));
 *
 * could be used to indicate to lockdep that foo->bar may only be dereferenced
 * if either rcu_read_lock() is held, or that the lock required to replace
 * the bar struct at foo->bar is held.
 *
 * Note that the list of conditions may also include indications of when a lock
 * need not be held, for example during initialisation or destruction of the
 * target struct:
 *
 *	bar = rcu_dereference_check(foo->bar, lockdep_is_held(&foo->lock) ||
 *					      atomic_read(&foo->usage) == 0);
 *
 * Inserts memory barriers on architectures that require them
 * (currently only the Alpha), prevents the compiler from refetching
 * (and from merging fetches), and, more importantly, documents exactly
 * which pointers are protected by RCU and checks that the pointer is
 * annotated as __rcu.
 */
#define rcu_dereference_check(p, c) \
	__rcu_dereference_check((p), __UNIQUE_ID(rcu), \
				(c) || rcu_read_lock_held(), __rcu)

#define rcu_assign_pointer(p, v) ((p) = (v))

#define rcu_barrier()

#define call_rcu(x, y)
