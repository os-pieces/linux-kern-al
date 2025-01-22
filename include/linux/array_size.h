/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <linux/compiler.h>

/**
 * ARRAY_SIZE - get the number of elements in array @arr
 * @arr: array to be sized
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
