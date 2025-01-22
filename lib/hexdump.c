#include <linux/hex.h>

static const char hex_asc[] = "0123456789abcdef";

#define hex_asc_lo(x)	hex_asc[((x) & 0x0f)]
#define hex_asc_hi(x)	hex_asc[((x) & 0xf0) >> 4]

char *hex_byte_pack(char *buf, unsigned char byte)
{
	*buf++ = hex_asc_hi(byte);
	*buf++ = hex_asc_lo(byte);

	return buf;
}

int hex2bin(u8 *dst, const char *src, size_t count)
{
	return 0;
}
