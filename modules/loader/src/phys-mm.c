//===================================================================================================================
//
//  loader physmm.c -- The Physical Memory Manager functions 
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
//  !!!IMPORTANT NOTE!!!:
//  Frames are not an addressed, but a number.  Frame number n is at address n << 12.  Keep that in mind!!
//
//  A frame is free if its flag is set; allocated or not available if the flag is clear.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-05-08  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


//#define DEBUG_PMM


#include "types.h"
#include "proto.h"
#include "mb-local.h"


//-------------------------------------------------------------------------------------------------------------------
// PhysMMInit() -- initialize the physical portion of the memory manager
//-------------------------------------------------------------------------------------------------------------------
void PhysMMInit(void)
{
    extern char _loaderStart[];
    extern char _loaderEnd[];
    arch_addr_t frame;
    arch_addr_t length;

    // -- Sanity check -- we cannot continue without a memory map
    if (!mbLocal.hasMmap) {
        kprintf(u8"PANIC: Unable to determine memory map; Century OS cannot initialize\n");
        Halt();
    }

    // -- OK, the first order of business is to determine how big and where to put the table
    uint32_t pages = (uint32_t)(GetMemAmount() >> (12 + 3 + 12)) + (GetMemAmount()&0x7fff?1:0);
    uint32_t start = GetEbda()?GetEbda():640*1024;

    start -= (pages << 12);
    start &= ~0x0fff;

    mbLocal.memBitMap = (uint32_t *)start; 
    mbLocal.memSizeFrames = pages;

    // -- pages now holds the bitmap aligned to 4K right up to the EBDA or 640K boundary; set to no available memory
    memset((void *)start, 0, pages << 12);

    // -- now we loop through the available memory and set the frames to be available
    for (uint32_t i = 0; i < mbLocal.numMmapEntries; i ++) {
        if (mbLocal.mmap[i].type == MMAP_FREE) {
            frame = mbLocal.mmap[i].baseAddr >> 12;
            length = mbLocal.mmap[i].length >> 12;

#ifdef DEBUG_PMM
            kprintf(u8"PHYSMM: Freeing frames from %p to %p of type %d\n", (uint32_t)frame, (uint32_t)(frame + length), 
                    mbLocal.mmap[i].type);
#endif
            FrameFreeRange(frame, length);

            if ((frame + length - 1) > mbLocal.mmu) mbLocal.mmu = frame + length - 1;
        }
    }

    // -- now that all our memory is available, set the loader space to be not available; _loader* already aligned
    FrameAllocRange(((uint32_t)_loaderStart) >> 12, ((uint32_t)_loaderEnd) >> 12);

    // -- Allocate the Frame Buffer
    FrameAllocRange(mbLocal.fbAddr, 1024 * 768 * 2);

    // TODO: allocate the loaded modules
    if (mbLocal.hasModulesLoaded) {
        for (uint32_t i = 0; i < mbLocal.numModulesLoaded; i ++) {
            frame = mbLocal.modules[i].modStart >> 12;
            length = (mbLocal.modules[i].modEnd >> 12) - frame;

#ifdef DEBUG_PMM
            kprintf(u8"PHYSMM: Allocating frames from %p to %p called %s\n", (uint32_t)frame, (uint32_t)(frame + length), 
                    &mbLocal.modules[i].modName[0]);
#endif
            FrameFreeRange(frame, length);
        }
    }

    // -- Finally, we have to mark the bitmap itself as used
    FrameAllocRange(start >> 12, pages >> 12);

    kprintf(u8"Phyiscal Memory Manager Initialized\n");
}


//-------------------------------------------------------------------------------------------------------------------
// FrameFree() -- Mark a frame as free (set the flag)
//-------------------------------------------------------------------------------------------------------------------
void FrameFree(arch_addr_t frame) 
{ 
    mbLocal.memBitMap[frame >> 5] |= (1 << (frame & 0x1f)); 
}


//-------------------------------------------------------------------------------------------------------------------
// FrameAlloc() -- Mark a frame as allocated (clear the flag)
//-------------------------------------------------------------------------------------------------------------------
void FrameAlloc(arch_addr_t frame) 
{ 
    mbLocal.memBitMap[frame >> 5] &= (~(1 << (frame & 0x1f))); 
}


//-------------------------------------------------------------------------------------------------------------------
// IsFrameAlloc() -- Returns if a frame is allocated
//-------------------------------------------------------------------------------------------------------------------
bool IsFrameAlloc(arch_addr_t frame) 
{ 
    uint32_t chk = mbLocal.memBitMap[frame >> 5] & (1 << (frame & 0x1f));
    return chk == 0;
}


//-------------------------------------------------------------------------------------------------------------------
// FrameNew() -- Allocate a frame for use (not going to do this too much here...)
//-------------------------------------------------------------------------------------------------------------------
arch_addr_t FrameNew(void)
{
    uint32_t frame = (uint32_t)mbLocal.mmu;

    while (true && frame != 0) {
        if (!IsFrameAlloc(frame)) {
            FrameAlloc(frame);
            return frame;
        } else frame --;
    }

    return -1;
}


//-------------------------------------------------------------------------------------------------------------------
// FrameFreeRange() -- Mark a range of frames as free (set the flag)
//-------------------------------------------------------------------------------------------------------------------
void FrameFreeRange(arch_addr_t frame, arch_addr_t len) 
{ 
    arch_addr_t end = frame + len;

    while (frame < end) FrameFree(frame ++);
}


//-------------------------------------------------------------------------------------------------------------------
// FrameAllocRange() -- Mark a range of frames as allocated (clear the flag)
//-------------------------------------------------------------------------------------------------------------------
void FrameAllocRange(arch_addr_t frame, arch_addr_t len) 
{ 
    arch_addr_t end = frame + len;

    while (frame < end) FrameAlloc(frame ++);
}


