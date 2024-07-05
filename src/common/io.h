/*
  This header file defines functions for handling file descriptor operations.
*/
#pragma once

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static inline bool wrapFclose(FILE** pfile)
{
    assert(pfile);
    if (!*pfile)
        return false;
    fclose(*pfile);
    return true;
}
// __attribute__ closes the fd automatically when the fd is out of the scope of function
#define AUTO_CLOSE_FD __attribute__((__cleanup__(wrapFclose)))