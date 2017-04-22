//===================================================================================================================
//
//  loader frame-buffer.h -- The common structs to manipulate the frame buffer and its contents
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
//  2017-04-19  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __FRAME_BUFFER_H_INCLUDED__
#define __FRAME_BUFFER_H_INCLUDED__


#include "types.h"


//
// -- This is the common implementation of the frame buffer control structure
//    -----------------------------------------------------------------------
struct FrameBufferInfo {
    uint16_t *buffer;
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
    uint32_t pitch; 
};


//
// -- This is the common declaration of the frame buffer control structure
//    --------------------------------------------------------------------
extern struct FrameBufferInfo frameBufferInfo;


//
// -- This is the internally linked system monospace font
//    ---------------------------------------------------
extern uint8_t systemFont[];


#endif
