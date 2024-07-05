#pragma once

#include "util/strbuf.h"

#define NUM_OS_RELEASE 7 // refer the struct 'my_fetch_os_result'
#define PATH_OS_RELEASE "/etc/os-release"

typedef struct ParseInfo
{
    const char* title;
    strbuf* val;
} ParseInfo;

bool parse_os_release(ParseInfo* infos);