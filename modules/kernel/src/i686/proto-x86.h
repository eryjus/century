//===================================================================================================================
//
//  kernel i686/proto-x86.h -- These are prototypes that are specific to the x86-style architecture
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
//  2017-04-09  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __PROTO_I686_H_INCLUDED__
#define __PROTO_I686_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


//-------------------------------------------------------------------------------------------------------------------
// kernel/inc/i686/ioport.s
//-------------------------------------------------------------------------------------------------------------------
uint8_t ReadIoPort(uint32_t);
void WriteIoPort(uint32_t, uint8_t);


#endif