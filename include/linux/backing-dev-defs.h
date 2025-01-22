#pragma once

struct backing_dev_info
{
    unsigned long ra_pages;
    unsigned io_pages;
};
