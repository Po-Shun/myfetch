#include "os.h"

void my_fetch_detect_os_impl(my_fetch_os_result* result);

my_fetch_os_result my_fetch_os_detect()
{
    my_fetch_os_result result;
    strBufInit(&result.name); 
    strBufInit(&result.prettyName); 
    strBufInit(&result.id); 
    strBufInit(&result.idLike); 
    strBufInit(&result.version); 
    strBufInit(&result.versionID); 
    strBufInit(&result.codename); 
    my_fetch_detect_os_impl(&result);
    return result;
}