#include <linux/rwsem.h>

int up_write(struct rw_semaphore *sem)
{
    return 0;
}

int down_read_killable(struct rw_semaphore *sem)
{
    return 0;
}

void down_read(struct rw_semaphore *sem)
{

}
