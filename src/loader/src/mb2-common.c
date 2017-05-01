//===================================================================================================================
//
//  loader mb2-common.c -- Multiboot 2 functions that are common to multiple architectures
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
//  2017-04-29  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#define DEBUG_MB2

#include "types.h"
#include "proto.h"
#include "mb2.h"


//
// -- This is the multiboot 1 information structure (will remain NULL if not booted with MB1-compliant loader)
//    --------------------------------------------------------------------------------------------------------
struct Mb2Fixed *mb2Data = 0;



//-------------------------------------------------------------------------------------------------------------------
// ReadMB2Info() -- Read the multiboot 2 information from the data provided and store it locally
//-------------------------------------------------------------------------------------------------------------------
void ReadMB2Info(void)
{
    if (!mb2Data) return;
}