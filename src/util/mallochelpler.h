#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static inline void wrapfree(const void* ptr)
{
    assert(ptr);
    if(*(void**)ptr)
    {
        free(*(void**)ptr);
    }
}
#define AUTO_FREE __attribute__((__cleanup__(wrapfree)))