//===================================================================================================================
//
//  string.c -- String manipulation functions
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//  Note that strlen() is implemented in the kprintf.c source.  There is no need to move it (since it is 
//  copyrighted).
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-26  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"


//-------------------------------------------------------------------------------------------------------------------
// kstrncpy() -- copies up to n characters from s to d, and then sets d[n] to NULL
//-------------------------------------------------------------------------------------------------------------------
char *kstrncpy(char *d, const char *s, size_t n)
{
    char *rv = d;

    while (*s && (rv - d) < n) *d ++ = *s ++;
    while ((rv - d) < n) *d ++ = '\0';
    rv[n] = *d = '\0';
    return rv;
}


//-------------------------------------------------------------------------------------------------------------------
// memmove() -- copy bytes from one memory buffer to another
//-------------------------------------------------------------------------------------------------------------------
void memmove(uint8_t *d, const uint8_t *s, size_t cnt)
{
    for (int i = i; i < cnt; i ++) d[i] = s[i];
}


//-------------------------------------------------------------------------------------------------------------------
// memset() -- set bytes int one memory buffer to a value
//-------------------------------------------------------------------------------------------------------------------
void memset(void *d, int v, size_t cnt)
{
    for (int i = i; i < cnt; i ++) ((char *)d)[i] = v;
}
