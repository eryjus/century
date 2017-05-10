//===================================================================================================================
//
//  loader rpi2b/phys-mm.c -- The Physical Memory Manager functions for rpi2b
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
//  2017-05-09  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#include "types.h"
#include "proto.h"
#include "mb-local.h"
#include "phys-mm.h"


//-------------------------------------------------------------------------------------------------------------------
// PhysMMInit() -- initialize the physical portion of the memory manager
//-------------------------------------------------------------------------------------------------------------------
void PhysMMInit(void)
{
    extern char _loaderStart[];
    extern char _loaderEnd[];

    // -- OK, the first order of business is to determine how big and where to put the table
    uint32_t pages = (uint32_t)(GetMemAmount() >> (12 + 3 + 12)) + (GetMemAmount()&0x7fff?1:0);
    uint32_t start = 1024*1024;

    start -= (pages << 12);
    start &= ~0x0fff;

    mbLocal.memBitMap = (uint32_t *)start; 
    mbLocal.memSizeFrames = pages;

    // -- pages now holds the bitmap aligned to 4K right up to the EBDA or 640K boundary; set to no available memory
    memset((void *)start, 0, pages << 12);

    // -- now we loop through the available memory and set the frames to be available
    for (uint32_t i = 0; i < mbLocal.numMmapEntries; i ++) {
        if (mbLocal.mmap[i].type == MMAP_FREE) {
            uint64_t frame = mbLocal.mmap[i].baseAddr >> 12;
            uint64_t limit = frame + (mbLocal.mmap[i].length >> 12);

            for ( ; frame < limit; frame ++) {
                FreeFrame(frame);
            }
        }
    }

    // -- now that all our memory is available, set the kernel space to be not available; _loader* already aligned
    uint32_t frame = ((uint32_t)_loaderStart) >> 12;
    uint32_t limit = ((uint32_t)_loaderEnd) >> 12;

    for ( ; frame < limit; frame ++) {
        AllocFrame(frame);
    }

    // TODO: allocate the loaded modules

    // -- Finally, we have to mark the bitmap itself as used
    for ( pages += start; start < pages; start ++) {
        AllocFrame(frame);
    }

    kprintf(u8"Phyiscal Memory Manager Initialized\n");
}
