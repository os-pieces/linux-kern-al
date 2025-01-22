#pragma once

static inline u16 get_unaligned_le16(const void *p)
{
	return *((const u16 *)p);
}

static inline u32 get_unaligned_le32(const void *p)
{
	return *((const u32 *)p);
}
