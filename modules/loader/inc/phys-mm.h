//===================================================================================================================
//
//  loader phys-mm.h -- The Physical Memory Manager
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
//  2017-05-07  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __PHYS_MM_H_INCLUDED__
#define __PHYS_MM_H_INCLUDED__


#include "types.h"
#include "proto.h"
#include "mb-local.h"


inline static void FreeFrame(uint64_t frame) { mbLocal.memBitMap[frame >> 5] |= (1 << (frame & 0x1f)); }
inline static void AllocFrame(uint64_t frame) { mbLocal.memBitMap[frame >> 5] &= (~(1 << (frame & 0x1f))); }


#endif
