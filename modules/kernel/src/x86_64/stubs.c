//===================================================================================================================
//
//  kernel x86_64/stubs.c -- This file contains the stubs required to compile the x86_64
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
// This is an empty file as a placeholder for the moment.  
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-10  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include <stdint.h>

void UartDevInit(void) {}
unsigned char UartGetC(void) { return 0; }
void UartPutC(const unsigned char byte) {}
void UartPutS(const char *str) {}
uint64_t ReadSysTimerCount(void) { return 0; }
