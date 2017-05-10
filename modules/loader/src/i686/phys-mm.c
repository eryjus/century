//===================================================================================================================
//
//  loader i686/phys-mm.c -- The Physical Memory Manager functions for i686
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//  This is really a watered-down version of the same memory manager that will be used by the kernel.  The reason
//  is that we really do not need to "manage" memory, just set its initial state.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-05-08  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#include "types.h"
#include "proto.h"


#include "x86/physmm.c"
