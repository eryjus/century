//===================================================================================================================
//
//  loader rpi2b/framebuffer.h -- These are the structs and functions to manipulate the frame buffer and its contents
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


#ifndef __FRAMEBUFFER_H_INCLUDED__
#define __FRAMEBUFFER_H_INCLUDED__


//
// -- This frame buffer structure is used to pass information back and forth to the GPU
//    ---------------------------------------------------------------------------------
struct FrameBufferInfo {
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
} __attribute__((aligned(4)));


#endif
