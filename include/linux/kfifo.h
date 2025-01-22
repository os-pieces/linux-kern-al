#pragma once

#include <asm/barrier.h>

struct __kfifo
{
    unsigned char *data;
    unsigned int in;
    unsigned int out;
    unsigned int mask;
    unsigned int esize;
    unsigned short in_end;
    unsigned short out_end;
};

int __kfifo_init(struct __kfifo *fifo, void *buffer,
                 unsigned int size, unsigned int esize);
unsigned int __kfifo_in(struct __kfifo *fifo,
                        const void *buf, unsigned int len);
unsigned int __kfifo_out(struct __kfifo *fifo, void *buf, unsigned int len);
unsigned int kfifo_unused(struct __kfifo *fifo);

#define	kfifo_get(fifo, val) __kfifo_out(fifo, val, sizeof(*val))

#define	kfifo_is_empty(fifo) ((fifo)->in == (fifo)->out)
#define	kfifo_in(fifo, buf, n) __kfifo_in(fifo, buf, n)

#define kfifo_size(fifo)	((fifo)->mask + 1)
#define kfifo_len(fifo) ((fifo)->in - (fifo)->out)

#define	kfifo_avail(fifo) kfifo_unused(fifo)
