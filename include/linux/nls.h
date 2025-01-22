#pragma once

typedef unsigned short wchar_t;

/* this value hold the maximum octet of charset */
#define NLS_MAX_CHARSET_SIZE 6 /* for UTF-8 */

/* this value hold the maximum octet of charset */
#define NLS_MAX_CHARSET_SIZE 6 /* for UTF-8 */

/* Byte order for UTF-16 strings */
enum utf16_endian {
	UTF16_HOST_ENDIAN,
	UTF16_LITTLE_ENDIAN,
	UTF16_BIG_ENDIAN
};

struct nls_table
{
	const char *charset;
    int (*uni2char)(wchar_t uni, unsigned char *out, int boundlen);
    int (*char2uni)(const unsigned char *rawstring, int boundlen, wchar_t *uni);
	const unsigned char *charset2lower;
	const unsigned char *charset2upper;
};

extern int utf8s_to_utf16s(const unsigned char *s, int len,
		enum utf16_endian endian, wchar_t *pwcs, int maxlen);

static inline unsigned char nls_tolower(struct nls_table *t, unsigned char c)
{
	unsigned char nc = t->charset2lower[c];

	return nc ? nc : c;
}

static inline unsigned char nls_toupper(struct nls_table *t, unsigned char c)
{
	unsigned char nc = t->charset2upper[c];

	return nc ? nc : c;
}

extern struct nls_table *load_nls(const char *charset);
extern void unload_nls(struct nls_table *);
extern int utf16s_to_utf8s(const wchar_t *pwcs, int len,
		enum utf16_endian endian, unsigned char *s, int maxlen);
int nls_strnicmp(struct nls_table *t, const unsigned char *s1,
		const unsigned char *s2, int len);
