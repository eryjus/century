//===================================================================================================================
//
//  loader mmio.h -- This is the basic interface to read/write to mmio registers.
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

#ifndef __MMIO_H_INCLUDED__
#define __MMIO_H_INCLUDED__


#include "types.h"


//-------------------------------------------------------------------------------------------------------------------
// MmioWrite() -- Write to a Memory Mapped I/O Register
//
// You better know what you are writing and to where.  There are no sanity checks here!
//-------------------------------------------------------------------------------------------------------------------
static inline void MmioWrite(arch_addr_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}


//-------------------------------------------------------------------------------------------------------------------
// MmioRead() -- Read from a Memory Mapped I/O Register
//
// You better know where you are reading from.  There are no sanity checks here!
//-------------------------------------------------------------------------------------------------------------------
static inline uint32_t MmioRead(arch_addr_t reg)
{
    return *(volatile uint32_t *)reg;
}

#endif
