//===================================================================================================================
//
//  loader rpi2b/mmu.c -- The functions for interfacing with the Memory Management Unit (MMU)
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
//  2017-05-26  Initial   0.0.0   ADCL  Initial version (second iteration)
//
//===================================================================================================================


#include "mmu.h"
#include "proto.h"
#include "mb-local.h"

//#define DEBUG_MMU

//
// -- Define some constants to help us shift the right number of bits
//    ---------------------------------------------------------------
enum {
    MMU_TTL1_BITS = 20,
    MMU_TTL2_BITS = 12,
};


//
// -- This is the pointer to the Ttl1 table
//    -------------------------------------
struct Ttl1 *ttl1 = 0;


//-------------------------------------------------------------------------------------------------------------------
// NewTtl1Addr() -- Obtain the frames for the mmu ttl1 and store it back in the data structure
//-------------------------------------------------------------------------------------------------------------------
static struct Ttl1 *NewTtl1Addr(void)
{
    if ((mbLocal.mmu & 0x03) != 0x03) mbLocal.mmu -= 0x03;
    mbLocal.mmu &= (~ 0x03);                             // this will align the table start to 0b00 with enough room
    FrameAllocRange(mbLocal.mmu, 4);
    uint32_t frame = (uint32_t)(mbLocal.mmu << 12);
    memset((void *)frame, 0, 4 * 4096);
    ttl1 = (struct Ttl1 *)frame;

#ifdef DEBUG_MMU
    kprintf(u8"MMU: The resulting Ttl1 frame starts at %p\n", mbLocal.mmu);
#endif

    return ttl1;
}


//-------------------------------------------------------------------------------------------------------------------
// GetRecursiveTtl2Entry() -- Get the "recursive" Ttl2 Entry
//-------------------------------------------------------------------------------------------------------------------
struct Ttl2 *GetRecursiveTtl2Entry(arch_addr_t addr)
{
    struct Ttl1 *t1e = &ttl1[(addr >> 30) + 4092];
    struct Ttl2 *t2 = (struct Ttl2 *)(t1e->ttl2 << 10);
    struct Ttl2 *t2Entry = &t2[(addr >> 12) & 0x3ff];

    return t2Entry;
}


//-------------------------------------------------------------------------------------------------------------------
// NewTtl2Table() -- Obtain a frame for a Ttl2 table and initialize it to 4 Ttl1 entries
//
// NOTE: this should work during initialization.
//-------------------------------------------------------------------------------------------------------------------
void NewTtl2Table(arch_addr_t addr)
{
#ifdef DEBUG_MMU
    kprintf(u8"MMU: Request to create a level 2 table for address %p\n", addr);
#endif
    // -- This bit establishes the new TTL2 table
    uint32_t index = (addr >> MMU_TTL1_BITS) & (~ 0x03);
    uint32_t frame = FrameNew();
    memset((void *)(frame << 12), 0, 4096);
    struct Ttl1 *ttl1Entry = &ttl1[index];

    ttl1Entry[0].ttl2 = (frame << 2);
    ttl1Entry[0].fault = 0x01;

    ttl1Entry[1].ttl2 = (frame << 2) + 1;
    ttl1Entry[1].fault = 0x01;
    
    ttl1Entry[2].ttl2 = (frame << 2) + 2;
    ttl1Entry[2].fault = 0x01;
    
    ttl1Entry[3].ttl2 = (frame << 2) + 3;
    ttl1Entry[3].fault = 0x01;
    
    // -- This bit maps the new table into the pseudo-recursive section
    struct Ttl2 *t2Entry = GetRecursiveTtl2Entry(addr);
#ifdef DEBUG_MMU
    kprintf(u8"MMU:   The address of the recursive entry is at %p\n", t2Entry);
#endif

    t2Entry->fault = 0b10;
    t2Entry->b = 0;
    t2Entry->c = 0;
    t2Entry->ap = 0;
    t2Entry->tex = 0;
    t2Entry->apx = 0;
    t2Entry->s = 1;
    t2Entry->nG = 0;
    t2Entry->frame = frame;
}


//-------------------------------------------------------------------------------------------------------------------
// GetTtl1Entry() -- From an address, get the Ttl1 Entry
//-------------------------------------------------------------------------------------------------------------------
static struct Ttl1 *GetTtl1Entry(arch_addr_t addr)
{
    return &ttl1[addr >> 20];
}


//-------------------------------------------------------------------------------------------------------------------
// GetTtl2Table() -- From a Ttl1 Entry, get the Ttl2 table address
//-------------------------------------------------------------------------------------------------------------------
static struct Ttl2 *GetTtl2Table(struct Ttl1 *t1)
{
    return (struct Ttl2 *)(t1->ttl2 << 10);
}


//-------------------------------------------------------------------------------------------------------------------
// GetTtl2Entry() -- From a Ttl2 Table and an address, get the Ttl2 entry address
//-------------------------------------------------------------------------------------------------------------------
static struct Ttl2 *GetTtl2Entry(struct Ttl2 *t2, arch_addr_t addr)
{
    return &t2[(addr >> 12) & 0xff];
}


//-------------------------------------------------------------------------------------------------------------------
// MmuMapAddress() -- Map a previously unmapped virtual address to an allocated frame
//-------------------------------------------------------------------------------------------------------------------
bool MmuMapAddress(arch_addr_t addr, arch_addr_t frame)
{
#ifdef DEBUG_MMU
    kprintf(u8"MMU: Request to map address %p to frame %p\n", addr, frame);
#endif

    if (!IsFrameAlloc(frame)) {
        kprintf(u8"MMU: Frame is not allocated\n");
        return false;
    }

    // -- Check for a TTL2 table
    struct Ttl1 *ttl1Entry = GetTtl1Entry(addr);
#ifdef DEBUG_MMU
    kprintf(u8"MMU: The TTL1 Entry is at %p\n", ttl1Entry);
#endif
    if (ttl1Entry->fault == 0b00) NewTtl2Table(addr);

    // -- now, we will get the proper TTL2 table address for management
    struct Ttl2 *ttl2Entry = GetTtl2Entry(GetTtl2Table(ttl1Entry), addr);
    ttl2Entry->fault = 0b10;
    ttl2Entry->b = 0;
    ttl2Entry->c = 0;
    ttl2Entry->ap = 0;
    ttl2Entry->tex = 0;
    ttl2Entry->apx = 0;
    ttl2Entry->s = 1;
    ttl2Entry->nG = 0;
    ttl2Entry->frame = frame;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MmuTablesInit() -- Perform the required initialization
//-------------------------------------------------------------------------------------------------------------------
void MmuTablesInit(void)
{
    extern char _loaderStart[];
    extern char _loaderEnd[];
    uint32_t frame;

    // -- First we need to allocate a frame for the TTl1 Table; this will be the physical address for the MMU.
    //    For this we will take the highest available frame on the system (within the 32-bit address range).
    //    ----------------------------------------------------------------------------------------------------------
    NewTtl1Addr();

    // -- this section establishes the TTL2 tables for the pseudo-recursive ttl2 tables
    frame = FrameNew();
    memset((void *)(frame << 12), 0, 4096);

    ttl1[4092].ttl2 = (frame << 2);
    ttl1[4092].fault = 0x01;

    ttl1[4093].ttl2 = (frame << 2) + 1;
    ttl1[4093].fault = 0x01;
    
    ttl1[4094].ttl2 = (frame << 2) + 2;
    ttl1[4094].fault = 0x01;
    
    ttl1[4095].ttl2 = (frame << 2) + 3;
    ttl1[5095].fault = 0x01;

#ifdef DEBUG_MMU
    kprintf(u8"MMU: The ttl1[508].ttl2 == part %d of frame %p\n", ttl1[4092].ttl2 & 0x03, ttl1[4092].ttl2 >> 2);
    kprintf(u8"MMU: The ttl1[509].ttl2 == part %d of frame %p\n", ttl1[4093].ttl2 & 0x03, ttl1[4093].ttl2 >> 2);
    kprintf(u8"MMU: The ttl1[510].ttl2 == part %d of frame %p\n", ttl1[4094].ttl2 & 0x03, ttl1[4094].ttl2 >> 2);
    kprintf(u8"MMU: The ttl1[511].ttl2 == part %d of frame %p\n", ttl1[4095].ttl2 & 0x03, ttl1[4095].ttl2 >> 2);
#endif
    
    // -- complete the initialization
    MmuMapAddress(MMU_TTL1, mbLocal.mmu);
    MmuMapAddress(MMU_TTL1 + 0x1000, mbLocal.mmu + 1);
    MmuMapAddress(MMU_TTL1 + 0x2000, mbLocal.mmu + 2);
    MmuMapAddress(MMU_TTL1 + 0x3000, mbLocal.mmu + 3);

    // Next, we identity-map the loader (required when we turn paging on)
    for (frame = ((uint32_t)_loaderStart) >> 12; frame < (((uint32_t)_loaderEnd) >> 12); frame ++) {
        if (!MmuMapAddress(frame << 12, frame)) {
            kprintf(u8"MMU: Failed to identity map the loader\n");
        }
    }

    // 
    // -- OK, there are a few additional things that will need to be mapped, some of which will be mapped
    //    below and others we will need to map in other sections.  Examples:
    //    * Map the frame buffer (done)
    //    * The PhysMM frame bitmap (which will be relocated before transferring control)
    //    * The kernel space (which should be able to be mapped here, once we have the memory map finalized)
    //    * The additional modules (should those be done in the kernel proper?)
    //    * The structure to pass data from the loader to the kernel (will be relocated so we will do this later)
    //    -------------------------------------------------------------------------------------------------------
    for (frame = 0; frame < 384; frame ++) {
        uint32_t a = (frame << 12) + MMU_FRAME_BUF;
        uint32_t f = frame + (mbLocal.fbAddr >> 12);
        if (!MmuMapAddress(a, f)) {
            kprintf(u8"MMU: Failed to map the frame buffer\n");
        }
    }

    kprintf(u8"Paging structures initialized\n");

    MmuDumpTables(0x0fffff);
    MmuDumpTables(0x100000);
    MmuDumpTables(0xffffffff);
    MmuDumpTables(MMU_FRAME_BUF);
    MmuDumpTables(MMU_TTL1);
    MmuDumpTables(MMU_TTL2);
}


//-------------------------------------------------------------------------------------------------------------------
// MmuDumpTables() -- Dump the MMU Tables for a specific address
//-------------------------------------------------------------------------------------------------------------------
void MmuDumpTables(arch_addr_t addr)
{
    kprintf(u8"\nMmuDumpTables: Walking the page tables for address 0x%08x\n", addr);
    kprintf(u8"Level  Address       Index   Frame       fault\n");
    kprintf(u8"-----  ----------    -----   --------    -----\n");

    uint32_t i = (addr >> MMU_TTL1_BITS) & 0xfff;
    struct Ttl1 *t1 = &ttl1[i];

    kprintf(u8"TTL1   0x%08x    0x%03x   0x%05x        %u\n", ttl1, i, (uint32_t)t1->ttl2, t1->fault);
    if (!t1->fault) return;
    struct Ttl2 *t2t = GetTtl2Table(t1);
    i = (addr >> MMU_TTL2_BITS) & 0xff;
    struct Ttl2 *t2 = &t2t[i];

    kprintf(u8"TTL2   0x%08x    0x%03x   0x%05x        %u\n", t2t, i, (uint32_t)t2->frame, t2->fault);
}
