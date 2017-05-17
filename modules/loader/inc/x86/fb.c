//===================================================================================================================
//
//  loader inc/x86/fb.c -- X86-specific framebuffer functions
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
//  2017-05-03  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"


//-------------------------------------------------------------------------------------------------------------------
// FrameBufferInit() -- Initialize the additional frame buffer info
//-------------------------------------------------------------------------------------------------------------------
bool FrameBufferInit(void)
{
#ifdef DEBUG_FB
    kprintf(u8"FB: Initialized the frame buffer address to %p\n", mbLocal.fbAddr);
#endif
    mbLocal.color = 0xffff;
    mbLocal.bgColor = 0x1234;
    FrameBufferClear();
    return true;
}
