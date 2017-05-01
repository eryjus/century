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


#include "types.h"
#include "proto.h"
#include "mb1.h"
#include "frame-buffer.h"


//-------------------------------------------------------------------------------------------------------------------
// FrameBufferInit() -- Initialize the frame buffer, setting up the expected resolution -- based on MB1 data
//-------------------------------------------------------------------------------------------------------------------
bool FrameBufferInit(void)
{
    if (mb1Data) {
        frameBufferInfo.buffer = (uint16_t *)(uint32_t)(mb1Data->fbAddr & 0xffffffff);
        frameBufferInfo.width = mb1Data->fbWidth;
        frameBufferInfo.height = mb1Data->fbHeight;
        frameBufferInfo.bpp = mb1Data->fbBpp;
        frameBufferInfo.pitch = mb1Data->fbPitch;

        if (frameBufferInfo.pitch == 0) frameBufferInfo.pitch = frameBufferInfo.width * frameBufferInfo.bpp / 8;

        FrameBufferClear();

        return true;
    }

    return false;
}


