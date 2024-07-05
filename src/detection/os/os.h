#pragma once

#include "util/strbuf.h"
typedef struct my_fetch_os_result
{
    strbuf name;
    strbuf prettyName;
    strbuf id;
    strbuf idLike;
    strbuf version;
    strbuf versionID;
    strbuf codename;
} my_fetch_os_result;

my_fetch_os_result my_fetch_os_detect();