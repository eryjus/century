//===================================================================================================================
//
//  kernel kmain.c -- This is the main kernel function, responsible for getting the kernel operational
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
#include "hw.h"


//-------------------------------------------------------------------------------------------------------------------
// kMain() -- this is the main C entry for the kernel
//-------------------------------------------------------------------------------------------------------------------
void kMain(uint32_t r0, uint32_t r1, uint32_t atags)
{
    UartDevInit();

    UartPutS("Hello, world!!");

    while (true) UartPutC(UartGetC());
}
