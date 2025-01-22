#pragma once

struct rw_semaphore {

};

int up_write(struct rw_semaphore *sem);
int down_read_killable(struct rw_semaphore *sem);
extern void down_read(struct rw_semaphore *sem);

#define init_rwsem(sem)
