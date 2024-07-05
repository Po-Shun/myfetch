#include "parse.h"
#include "io.h"
#include "util/mallochelpler.h"


bool parse_os_match_prop(const char** line, const char* title, strbuf* val)
{
    while(*title != '\0')
    {
        // the property is unmatched 
        if(**line != *title)
        {
            return false;
        }

        ++(*line);
        ++title;
    }
    char line_end = '\0';
    if(**line == '"') 
    {
        line_end = **line;
        ++(*line);
    }
    while(**line != line_end && **line != '\n' && **line != '\0')
    {
        strBufAppendChar(val, **line);
        ++(*line);
    }
    return true;
}


// sample output from /etc/os-release
// NAME="Ubuntu"
// VERSION="20.04 LTS (Focal Fossa)"
// ID=ubuntu
// ID_LIKE=debian
// PRETTY_NAME="Ubuntu 20.04 LTS"
// VERSION_ID="20.04"
// HOME_URL="https://www.ubuntu.com/"
// SUPPORT_URL="https://help.ubuntu.com/"
// BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
// PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
// VERSION_CODENAME=focal
// UBUNTU_CODENAME=focal
bool parse_os_release(ParseInfo* infos)
{
    AUTO_CLOSE_FD FILE* fp = fopen(PATH_OS_RELEASE, "r");

    if(fp == NULL){
        printf("File \"/etc/os-release\" doesn't exist\n");
        return false;
    } 

    // check list for the properties, true if unset
    bool check_unset[NUM_OS_RELEASE];

    // Initialize all elements to true
    for (int i = 0; i < NUM_OS_RELEASE; i++) {
        check_unset[i] = true;
    }
    
    AUTO_FREE char* line = NULL;
    size_t len = 0;

    while(getline(&line, &len, fp) != -1)
    {
        // find the matched property
        for(int i = 0; i < NUM_OS_RELEASE; i++)
        {
            // try to parse the property if the prop is already set (check is false)
            if(!check_unset[i])
            {
                continue;
            }

            // due to pointer type safety rule the next two conversion is necessary
            // alternative way write another fucntion take line as input and conver it to "const char**" finaaly 
            const char* p = line;
            const char** ref_line = &p;
            bool match_result = parse_os_match_prop(ref_line, infos[i].title, infos[i].val);
            if(!match_result)
                continue;
            check_unset[i] = false;
            break;
        }
    }    
    return true;
}