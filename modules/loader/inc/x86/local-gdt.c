//===================================================================================================================
//
//  loader inc/x86/local-gdt.c -- This file contains the local (temp) GDT that will be used by loader
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//    We define our local GDT here, which will get set pretty quickly on getting control.  Note that since this 
//    GDT exists in the .data section of our code, which is between 1M and 2M (between the loader and kernel),
//    and we expect to reclaim this portion, this can ONLY be a temporary GDT.  No matter what, we will build
//    a newer and more robust GDT for the kernel dynamically in a more permanent location and hand that off
//    to the kernel for its processing pleasure.  Also note: the arm architecture does not use this since it is 
//    only a flat memory model.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-05-06  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"

#include "gdt.h"


//
// -- The loader's GDT
//    ----------------
struct SegmentDescriptor32 gdt[3] = {
    SEG32(0, 0, 0, 0),                  // NULL descriptor
    SEG32(GDT_CODE, 0, 0xffffffff, 0),     // Code Selector
    SEG32(GDT_DATA, 0, 0xffffffff, 0),     // Data Selector
};  