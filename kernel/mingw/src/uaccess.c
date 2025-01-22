#include <linux/compiler.h>
#include <linux/uaccess.h>
#include <linux/printk.h>

#include <string.h>

long strncpy_from_user(char *dst, const char __user *src,
				    long count)
{
    long len;

    len = strlen(src);
    strncpy(dst, src, count);
    if (len > count)
        len = count;

    return len;
}

unsigned long copy_to_user(void __user *to, const void *from, unsigned long n)
{
    memcpy(to, from, n);

    return 0;
}

unsigned long copy_from_user(void *to, const void *from, unsigned long n)
{
    memcpy(to, from, n);

    return 0;
}
