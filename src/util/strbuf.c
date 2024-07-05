#include "strbuf.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>

char* CHAR_NULL_PTR = "";

void strBufInit_with_allocate(strbuf* s_buf, uint32_t allocate)
{
    s_buf->allocated = allocate;
    
    if(allocate > 0){
        s_buf->chars = (char*) malloc(sizeof(char) * allocate);
    }

    strBufClear(s_buf);
}

// set the length to zero and chars to null byte
void strBufClear(strbuf* s_buf){
    assert(s_buf != NULL);

    if(s_buf->allocated == 0)
        s_buf->chars = CHAR_NULL_PTR;
    else
        s_buf->chars[0] = '\0';
    
    s_buf->length = 0;
}

/*
    strBufEnsureLength
        parameter:
            strbuf* s_buf -> the pointer point to the strbuf
            uint32_t length -> the length of the incoming string
        description:
            the func is respondsible for the capacity check of the strbuf.
            if the capacity is not enough, the size should be doubled up until it catches the need
*/
void strBufEnsureLength(strbuf* s_buf, uint32_t length)
{
    // if the free space is already enough, just return it directly
    if (strBufGetFree(s_buf) >= length)
        return;
    
    uint32_t allocate = s_buf->allocated;
    if(allocate == 0)
        allocate = MYFETCH_SBUF_DEFAULT_ALLOCATE_SIZE;
    
    // if the size is not enough for the need, dobule up the size until it matches the need
    while(s_buf->length + length + 1 > allocate) // "+1" for the "\0"
        allocate *= 2;
    
    if(s_buf->allocated == 0)
    {
        char* s_buf_start = malloc(sizeof(char) * allocate);
        *s_buf_start = '\0';
        s_buf->chars = s_buf_start;
    }
    else
    {
        s_buf->chars = realloc(s_buf->chars, sizeof(char) * allocate);
    }

    s_buf->allocated = allocate;
}

void strBufAppend(strbuf* s_buf, uint32_t length, const char* value)
{
    if(value == NULL && length == 0)
        return;

    // ensure the buf have enough length to store the value 
    strBufEnsureLength(s_buf, length);
    memcpy(&s_buf->chars[s_buf->length], value, length);
    s_buf->length += length;
    s_buf->chars[length] = '\0';
}

void strBufAppendChar(strbuf* s_buf, const char value)
{
    strBufEnsureLength(s_buf, 1);
    s_buf->chars[s_buf->length] = value;
    ++s_buf->length;
    s_buf->chars[s_buf->length] = '\0';
}

void strBufAppendVariableArg(strbuf* s_buf, const char* format, va_list args)
{
    assert(format != NULL);
    
    // copy the args for reuse purpose
    va_list copy;
    va_copy(copy, args);

    uint32_t free = strBufGetFree(s_buf);
    // try to writing the format into s_buf via vnspintf
    int written = vsnprintf(s_buf->chars + s_buf->length, s_buf->allocated > 0 ? free + 1 : 0, format, args);

    // free space is not enough
    if(written > 0 && written > free)
    {
        strBufEnsureLength(s_buf, s_buf->length + written + 1); // +1 -> terminating null byte
        written = vsnprintf(s_buf->chars + s_buf->length, written + 1, format, copy);
    }
    va_end(copy);

    if(written > 0)
    {
        s_buf->length += (uint32_t)written;
    }
}

// "strBufAppendFormatSetup" -> setting up the ANSI escape code to control text formatting 
void strBufAppendFormatSetup(strbuf* s_buf, char* format, ...)
{
    assert(format != NULL);
    va_list args;
    va_start(args, format);
    strBufAppendVariableArg(s_buf, format, args);
    va_end(args);
}

void strBufAppendContiChar(strbuf* s_buf, const uint32_t size, const char value)
{
    strBufEnsureLength(s_buf, size);

    memset(s_buf->chars + s_buf->length, value, size);
    s_buf->length += size;
    s_buf->chars[s_buf->length] = '\0';
}