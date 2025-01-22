#pragma once

#include <linux/types.h>

char *hex_byte_pack(char *buf, unsigned char byte);
extern int hex2bin(u8 *dst, const char *src, size_t count);
