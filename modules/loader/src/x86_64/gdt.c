//===================================================================================================================
//
//  loader x86_64/gdt.c -- This is the GDT for the x86_64 architecture; which is x86 common architectures
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

#include "types.h"
#include "proto.h"

#include "x86/gdt.h"


//
// -- The loader's GDT
//    ----------------
struct SegmentDescriptor gdt[16] = {
    SEG_NULL,                                   // 0x00<<3: NULL descriptor
    SEG64(GDT_CODE, 0),                         // 0x01<<3: Kernel Code Selector
    SEG64(GDT_DATA, 0),                         // 0x02<<3: Kernel Stack (Data) Selector
    SEG64(GDT_CODE, 3),                         // 0x03<<3: User Code Selector
    SEG64(GDT_DATA, 3),                         // 0x04<<3: User Stack (Data) Selector
    SEG_NULL,                                   // 0x05<<3: Reserved for user data if desired
    SEG_NULL,                                   // 0x06<<3: Reserved for kernel data if desired
    SEG32(GDT_CODE, 0, 0xffffffff, 0),          // 0x07<<3: Loader Code Selector
    SEG32(GDT_DATA, 0, 0xffffffff, 0),          // 0x08<<3: Loader Data & Stack Selector
    SEG_NULL,                                   // 0x09<<3: TSS Part 1
    SEG_NULL,                                   // 0x0a<<3: TSS Part 2
    SEG_NULL,                                   // 0x0b<<3: Future use call gate
    SEG_NULL,                                   // 0x0c<<3: Future use call gate
    SEG_NULL,                                   // 0x0d<<3: Future use call gate
    SEG_NULL,                                   // 0x0e<<3: Future use call gate
    SEG_NULL,                                   // 0x0f<<3: Future use call gate
};  

