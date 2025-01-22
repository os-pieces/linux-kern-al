#pragma once

#define container_of(ptr, type, member) \
    ((type *)((void *)(ptr) - (void*)(&((type *)0)->member)))

