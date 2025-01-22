#pragma once

#include <asm/uaccess.h>

long strncpy_from_user(char *dst, const char *src,
				    long count);
unsigned long copy_to_user(void *to, const void *from, unsigned long n);
unsigned long copy_from_user(void *to, const void *from, unsigned long n);

#define unsafe_op_wrap(op, err) ({if (unlikely(op)) goto err;})

#define put_user(x, ptr) ({0;})

#define get_user(x, ptr) ({0;})

#define user_write_access_begin(a, s) ({1;})
#define user_write_access_end()

#define unsafe_put_user(x,p,e) unsafe_op_wrap(__put_user(x, p), e)
#define unsafe_copy_to_user(d,s,l,e) unsafe_op_wrap(copy_to_user(d,s,l),e)
