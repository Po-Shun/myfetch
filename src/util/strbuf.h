#pragma once

#include "macro.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MYFETCH_SBUF_DEFAULT_ALLOCATE_SIZE 32

/*

"strbuf" is a struct that maintain the info should be printed

variables:
    uint32_t allocated -> value that represents the maximum length of the string 
    unint32_t length -> index that record the end of the string
    char* chars -> store the start of the string itself

*/
typedef struct strbuf
{
    uint32_t allocated;
    uint32_t length;
    char* chars;
} strbuf;

static inline void strBufInit(strbuf* s_buf);
void strBufInit_with_allocate(strbuf* s_buf, uint32_t allocate);
void strBufClear(strbuf* s_buf);
void strBufAppend(strbuf* s_buf, uint32_t length, const char* value);
void strBufAppendChar(strbuf* s_buf, const char value);
void strBufAppendContiChar(strbuf* s_buf, const uint32_t size, const char value);
void strBufAppendFormatSetup(strbuf* s_buf, char* format, ...);
void strBufEnsureLength(strbuf* s_buf, uint32_t length);

DISCARD_WARN static inline strbuf strBufCreate()
{
    strbuf sbuf;
    strBufInit(&sbuf);
    return sbuf;
}

DISCARD_WARN static inline strbuf strBufCreatedWithAllocate(uint32_t size)
{
    strbuf sbuf;
    strBufInit_with_allocate(&sbuf, size);
    return sbuf;
}

DISCARD_WARN static inline uint32_t strBufGetFree(const strbuf* s_buf)
{
    assert(s_buf != NULL);
    if (s_buf->allocated == 0)
        return 0;
    
    // minus 1 (because the end of string is occupied by '\0')
    return s_buf->allocated - s_buf->length - 1;
}

static inline void strBufInit(strbuf* s_buf){
    extern char* CHAR_NULL_PTR;
    s_buf->allocated = 0;
    s_buf->length = 0;
    char* chars = CHAR_NULL_PTR;
}

static inline void strBufDestroy(strbuf* s_buf)
{
    extern char* CHAR_NULL_PTR;
    
    // buffer is originally empty
    if(s_buf->allocated == 0)
    {
        s_buf->length = 0;
        s_buf->chars = CHAR_NULL_PTR;
        return;
    }

    s_buf->allocated = s_buf->length = 0;
    free(s_buf->chars);
    s_buf->chars = CHAR_NULL_PTR;

}
#define STRBUF_AUTO_DESTROY strbuf __attribute__((__cleanup__(strBufDestroy)))