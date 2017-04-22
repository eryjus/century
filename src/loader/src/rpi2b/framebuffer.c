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

#define DEBUG_FRAMEBUFFER

#include "hw.h"
#include "frame-buffer.h"
#include "proto.h"

#include <stdbool.h>


//
// -- This frame buffer structure is used to pass information back and forth to the GPU
//    ---------------------------------------------------------------------------------
struct RPiFrameBufferInfo {
    int physWidth;
    int physHeight;
    int virtWidth;
    int virtHeight;
    int gpuBytesPerRow;
    int depth;
    int xOffset;
    int yOffset;
    int gpuPointer;
    int size;
} __attribute__((aligned(16)));
struct RPiFrameBufferInfo fbInfo;


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

    frameBufferInfo.buffer = (uint16_t *)((addr_t)(fbInfo.gpuPointer));
    frameBufferInfo.width = fbInfo.physWidth;
    frameBufferInfo.height = fbInfo.physWidth;
    frameBufferInfo.bpp = fbInfo.depth;
    frameBufferInfo.pitch = fbInfo.gpuBytesPerRow;

//    if (frameBufferInfo.pitch == 0) frameBufferInfo.pitch = frameBufferInfo.width * frameBufferInfo.bpp / 8;

    FrameBufferClear();

    return true;
}


