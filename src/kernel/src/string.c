//===================================================================================================================
//
//  kernel string.c -- String manipulation utilities, usually found in string.h
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-05  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "proto.h"


//-------------------------------------------------------------------------------------------------------------------
// strlen() -- Get the length of a NULL terminated string.
//-------------------------------------------------------------------------------------------------------------------
size_t strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++ s) { }
    return (s - str);
}