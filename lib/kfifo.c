#include <linux/kfifo.h>
#include <linux/log2.h>
#include <linux/errno.h>
#include <linux/minmax.h>
#include <linux/string.h>

unsigned int kfifo_unused(struct __kfifo *fifo)
{
    return (fifo->mask + 1) - (fifo->in - fifo->out);
}

int __kfifo_init(struct __kfifo *fifo, void *buffer,
                 unsigned int size, unsigned int esize)
{
    int ret = 0;

    if (!is_power_of_2(size))
        size = rounddown_pow_of_two(size);

    fifo->in = 0;
    fifo->out = 0;
    fifo->data = buffer;
    fifo->esize = esize;

    if (size < 2)
    {
        fifo->mask = 0;
        ret = -EINVAL;
    }
    else
    {
        fifo->mask = size - 1;
    }

    return ret;
}

static void kfifo_copy_in(struct __kfifo *fifo, const void *src,
                          unsigned int len, unsigned int off)
{
    unsigned int size = fifo->mask + 1;
    unsigned int esize = fifo->esize;
    unsigned int l;

    off &= fifo->mask;
    if (esize != 1)
    {
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = min(len, size - off);

    memcpy(fifo->data + off, src, l);
    memcpy(fifo->data, src + l, len - l);
    /*
     * make sure that the data in the fifo is up to date before
     * incrementing the fifo->in index counter
     */
    smp_wmb();
}

unsigned int __kfifo_in(struct __kfifo *fifo, const void *buf, unsigned int len)
{
    unsigned int l;

    l = kfifo_unused(fifo);
    if (len > l)
        len = l;

    if (len > 0)
    {
        kfifo_copy_in(fifo, buf, len, fifo->in);
        fifo->in += len;
    }

    return len;
}

static void kfifo_copy_out(struct __kfifo *fifo, void *dst,
                           unsigned int len, unsigned int off)
{
    unsigned int size = fifo->mask + 1;
    unsigned int esize = fifo->esize;
    unsigned int l;

    off &= fifo->mask;
    if (esize != 1)
    {
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = min(len, size - off);

    memcpy(dst, fifo->data + off, l);
    memcpy(dst + l, fifo->data, len - l);
    /*
     * make sure that the data is copied before
     * incrementing the fifo->out index counter
     */
    smp_wmb();
}

unsigned int __kfifo_out_peek(struct __kfifo *fifo,
                              void *buf, unsigned int len)
{
    unsigned int l;

    l = fifo->in - fifo->out;
    if (len > l)
        len = l;

    if (len > 0)
    {
        kfifo_copy_out(fifo, buf, len, fifo->out);
    }

    return len;
}

unsigned int __kfifo_out(struct __kfifo *fifo, void *buf, unsigned int len)
{
    len = __kfifo_out_peek(fifo, buf, len);
    fifo->out += len;

    return len;
}
