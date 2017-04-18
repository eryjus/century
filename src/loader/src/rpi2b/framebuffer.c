//===================================================================================================================
//
//  loader rpi2b/framebuffer.c -- These are the functions to manipulate the frame buffer and its contents
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
//  2017-04-17  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "hw.h"
#include "framebuffer.h"
#include "proto.h"

#include <stdbool.h>

//
// -- This is the data structure holding critical framebuffer information
//    -------------------------------------------------------------------
static struct FrameBufferInfo fbInfo;


//-------------------------------------------------------------------------------------------------------------------
// FrameBufferInit() -- Initialize the frame buffer, setting up the expected resolution
//-------------------------------------------------------------------------------------------------------------------
bool FrameBufferInit(void)
{
    fbInfo.physWidth = fbInfo.virtWidth = 1280;
    fbInfo.physHeight = fbInfo.virtHeight = 1024;
    fbInfo.gpuBytesPerRow = 0;
    fbInfo.depth = 16;
    fbInfo.xOffset = fbInfo.yOffset = 0;
    fbInfo.gpuPointer = 0;
    fbInfo.size = 0;

    MailboxSend(((uint32_t)&fbInfo) + 0x40000000, 1);

    if (MailboxReceive(1) != 0) return false;

    fbInfo.gpuPointer -= 0x40000000;
    FrameBufferClear();

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// FrameBufferClear() -- Clear the screen defined by the frame buffer
//-------------------------------------------------------------------------------------------------------------------
void FrameBufferClear(void)
{
    //
    // -- calculate the number of 16-bit words to write (rows * cols)
    //    -----------------------------------------------------------------------
    int cnt = fbInfo.physHeight * fbInfo.physWidth;
    uint16_t *b = (uint16_t *)fbInfo.gpuPointer;

    for (int i = 0; i < cnt; i ++) {
        b[i] = 0x0000;
    }
}
