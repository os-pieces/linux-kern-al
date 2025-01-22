#pragma once

#include <linux/xarray.h>

struct ida
{
    struct xarray xa;
};
