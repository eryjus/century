//===================================================================================================================
//
//  loader inc/x86/gdt.h -- This file contains the definitions for setting up the GDT
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
//  2017-05-06  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __GDT_H_INCLUDED__
#define __GDT_H_INCLUDED__


#include "types.h"


enum GdtTypes {
    GDT_DATA = 0b0010,
    GDT_CODE = 0b1010,
};


//
// -- This is a 32-bit mode segment descriptor.  It will be the only one used by the 32-bit systems, and the 
//    only one used by the loader.  In 64-bit systems, the loader will populate a 64-bit version, but will
//    only activate it when jumping to the kernel code to hand over control.
//    ------------------------------------------------------------------------------------------------------
struct SegmentDescriptor32 {
    unsigned int limitLow : 16;         // Low bits (15-0) of segment limit
    unsigned int baseLow : 16;          // Low bits (15-0) of segment base address
    unsigned int baseMid : 8;           // Middle bits (23-16) of segment base address
    unsigned int type : 4;              // Segment type (see GDT_* constants)
    unsigned int s : 1;                 // 0 = system, 1 = application (1 for code/data)
    unsigned int dpl : 2;               // Descriptor Privilege Level
    unsigned int p : 1;                 // Present (must be 1)
    unsigned int limitHi : 4;           // High bits (19-16) of segment limit
    unsigned int avl : 1;               // Unused (available for software use)
    unsigned int rsv1 : 1;              // Reserved (set to 0)
    unsigned int db : 1;                // 0 = 16-bit segment, 1 = 32-bit segment
    unsigned int g : 1;                 // Granularity: limit scaled by 4K when set
    unsigned int baseHi : 8;            // High bits (31-24) of segment base address
};


//
// -- This macro will be used to statically define the segments used by the loader.  Note that the segments
//    used by the kernel and OS will be dynamically built and therefore will not use these macros.
//    -----------------------------------------------------------------------------------------------------
#define SEG32(type,base,lim,dpl)                                                            \
(struct SegmentDescriptor32) {                                                              \
    ((lim) >> 12) & 0xffff,                 /* limitLow */                                  \
    (unsigned int)(base) & 0xffff,          /* baseLow */                                   \
    ((unsigned int)(base) >> 16) & 0xff,    /* baseMid */                                   \
    type,                                   /* type (see GDT_* constants) */                \
    1,                                      /* s: application segment */                    \
    dpl,                                    /* dpl */                                       \
    1,                                      /* p: always present */                         \
    (unsigned int)(lim) >> 28,              /* limitHi */                                   \
    0,                                      /* avl: keep as 0 for now */                    \
    0,                                      /* rsv1: must be 0 */                           \
    1,                                      /* db: 1 for 32-bit segment */                  \
    1,                                      /* g: 1 for limit * 4K */                       \
    (unsigned int)(base) >> 24              /* baseHi */                                    \
}


#endif