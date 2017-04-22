//===================================================================================================================
//
//  loader mb1-common.c -- Multiboot functions that are common to multiple architectures
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
//  2017-04-18  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#define DEBUG_MB1

#include "mb1.h"
#include "proto.h"


//
// -- This is the multiboot 1 information structure (will remain NULL if not booted with MB1-compliant loader)
//    --------------------------------------------------------------------------------------------------------
struct Multiboot1Info *mb1Data = 0;


//-------------------------------------------------------------------------------------------------------------------
// ReadMB1Info() -- Read the multiboot information from the data provided and store it locally
//-------------------------------------------------------------------------------------------------------------------
void ReadMB1Info(void)
{
    if (!mb1Data) return;

#ifdef DEBUG_MB1
    kprintf("Reading Multiboot1 Information (we have a structure at 0x%p\n", mb1Data);
#endif

    //
    // -- Read the VBE information if available
    //    -------------------------------------
    if (!(mb1Data->flags & (1 << 11))) {
        Halt();
    }
}
