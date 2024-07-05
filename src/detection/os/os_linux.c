#include "os.h"
#include "common/parse.h"

#include <string.h>
#include <stdlib.h>

void detectos(my_fetch_os_result* result)
{
    bool parse_result = parse_os_release((ParseInfo[]){
        {"NAME=", &result->name},
        {"VERSION=", &result->version},
        {"ID=", &result->id},
        {"ID_LIKE=", &result->idLike},
        {"PRETTY_NAME=", &result->prettyName},
        {"VERSION_ID=", &result->versionID},
        {"VERSION_CODENAME=", &result->codename}
    });
}

void my_fetch_detect_os_impl(my_fetch_os_result* result)
{
    detectos(result);
}