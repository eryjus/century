//===================================================================================================================
//
//  kernel mmio.c -- This is the basic interface to read/write to mmio registers.
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
//  2017-04-04  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#include "hw.h"
#include "proto.h"

//-------------------------------------------------------------------------------------------------------------------
// WriteMmio() -- Write to a Memory Mapped I/O Register
//
// You better know what you are writing and to where.  There are no sanity checks here!
//-------------------------------------------------------------------------------------------------------------------
void WriteMmio(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}


//-------------------------------------------------------------------------------------------------------------------
// ReadMmio() -- Read from a Memory Mapped I/O Register
//
// You better know where you are reading from.  There are no sanity checks here!
//-------------------------------------------------------------------------------------------------------------------
uint32_t ReadMmio(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}