//===================================================================================================================
//
//  kernel i686/hwtypes.h -- Some types needed in the OS that are really hardware specific
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
//  2017-04-10  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __HWTYPES_H_INCLUDED__
#define __HWTYPES_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef uint32_t addr_t;                    // This is the size of an address in this architecture

#endif