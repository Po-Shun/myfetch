#include "myfetch.h"
#include "detection/os/os.h"
#include "logo/logo.h"


#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

void run(){
    // detect the os
    my_fetch_os_result os = my_fetch_os_detect();
    // set up the corresponding logo
    my_fetch_logo* logo = setupLogoBuiltin(os.id.chars);
    LogoPrint(logo->lines);
}

int main(){
    strbuf s_buf = strBufCreate();
    strBufAppend(&s_buf, strlen(MYFETCH_DATATEXT_STRUCTURE), MYFETCH_DATATEXT_STRUCTURE);
    run();
    return 0;
}