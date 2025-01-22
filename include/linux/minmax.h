#pragma once


/**
 * min - return minimum of two values of the same or compatible types
 * @x: first value
 * @y: second value
 */
#define min(x, y)	(x < y ? x : y)

/**
 * max - return maximum of two values of the same or compatible types
 * @x: first value
 * @y: second value
 */
#define max(x, y)	(x > y ? x : y)

#define min_t(type, x, y) min((type)(x), (type)(y))
#define max_t(type, x, y) max((type)(x), (type)(y))
