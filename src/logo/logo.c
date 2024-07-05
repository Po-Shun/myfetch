#include "logo.h"
#include "detection/os/os.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

my_fetch_logo* ifLogoExist(const my_fetch_logo* clutch, const char* osID){
    for (const my_fetch_logo* idx = clutch; *idx->names != NULL; idx++) {
        if(strcasecmp(*idx->names, osID) == 0){
            return (my_fetch_logo*)idx;
        }
    }
    return NULL;
}

my_fetch_logo* setupLogoBuiltin(const char* osID)
{
    // search the clutch with the same start charactor of osid
    const my_fetch_logo* clutch = logoBuiltin[toupper(osID[0]) - 'A'];
    my_fetch_logo* ret = NULL;
    // find the exact osID in the clutch
    ret = ifLogoExist(clutch, osID);
    return ret;
}

void InitLogoState(logo_state* state)
{
    state->logoHeight = 0;
}

/* 
    Uft8Handler is the handler function to determine the bytes used to constitute an uft8 code.
    It determines the number of bytes in the UTF-8 character based on the specific bits set in the first byte. 
    This allows the function to correctly handle characters that are encoded with one to four bytes.

    Details:
    A UTF-8 encoded character can consist of one to four bytes. 
    The number of bytes is indicated by the leading bits in the first byte of the character:

    1 byte: 0xxxxxxx (0 to 127)
    2 bytes: 110xxxxx 10xxxxxx
    3 bytes: 1110xxxx 10xxxxxx 10xxxxxx
    4 bytes: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    The function reads the first byte and uses bitwise operations to determine the total number of bytes that make up the UTF-8 character. 
    It then appends each byte of the character to the result buffer.
*/
uint8_t Utf8Handler(const char* data){
    int32_t codepoint = (unsigned char) *data;
    
    if(codepoint < 127)
        return 1;
    else if ((codepoint & 0xE0) == 0xC0)
        return 2;
    else if ((codepoint & 0xF0) == 0xE0)
        return 3;
    else if ((codepoint & 0xF8) == 0xF0)
        return 4;
    else
        return 1; // Invalid uff8, print it as is, byte by byte
}

void LogoPrint(const char* data)
{
    InitLogoState(&state);
    STRBUF_AUTO_DESTROY result = strBufCreatedWithAllocate(2048);
    uint32_t cur_len = 0;
    strBufAppendFormatSetup(&result, "\e[%sm", COLOR_RED);
    // preprocess the logo printout
    while(*data != '\0')
    {
        // whenever meet the end of line
        if(*data == '\n' || *data == '\r' && *(data + 1) == '\n')
        {
            strBufAppendContiChar(&result, LOGO_PADDING_RIGHT, LOGO_PADDING_CHAR);
            
            // handling the case of '\r\n' scenerio
            if(*data == '\r')
            {
                ++data;
            }

            strBufAppendChar(&result, '\n');

            ++ data;
            // increase the logoheight
            state.logoHeight ++;
            continue;
        }

        // always print the '\t' as 4 spaces
        if(*data == '\t')
        {
            strBufAppendContiChar(&result, 4, ' ');
            ++data;
            continue;
        }

        // case of ANSI escape code
        if(*data == '\e' && *(data + 1) == '[')
        {
            strBufAppend(&result, strlen(ANSI_ESCAPE_CODE), ANSI_ESCAPE_CODE);
            data += 2;

            // append the control sequece
            if(isascii(*data))
            {
                strBufAppendChar(&result, *data);
                ++data;
                continue;
            }
        }

        // utf-8 handling
        uint8_t bytes = Utf8Handler(data);

        for(uint8_t i = 0; i < bytes; i++)
        {
            if(*data == '\0')
                break;
            strBufAppendChar(&result, *data++);
        }
    }
    strBufAppendFormatSetup(&result, "\e[1G\e[%uA", state.logoHeight);

}