//===================================================================================================================
//
//  loader x86_64/mmu.c -- The functions for interfacing with the Memory Management Unit (MMU)
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
//  2017-05-23  Initial   0.0.0   ADCL  Initial version (second iteration)
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
    MMU_PML4_BITS = 39,
    MMU_PDPT_BITS = 30,
    MMU_PD_BITS = 21,
    MMU_PT_BITS = 12,
};


//
// -- emulate the global cr3 register to make things easier
//    -----------------------------------------------------
struct PageEntry *cr3 = 0;


//-------------------------------------------------------------------------------------------------------------------
// NewPml4Addr() -- Obtain the frame address for the mmu and store it back in the data structure
//-------------------------------------------------------------------------------------------------------------------
static struct PageEntry *NewPml4Addr(void)
{
    mbLocal.mmu = FrameNew();
    uint32_t frame = (uint32_t)((mbLocal.mmu << 12) & 0xffffffff);
    memset((void *)frame, 0, 4096);
    cr3 = (struct PageEntry *)frame;

#ifdef DEBUG_MMU
    kprintf(u8"MMU: The resulting PML4 frame is %p\n", mbLocal.mmu);
#endif

    return cr3;
}


//-------------------------------------------------------------------------------------------------------------------
// GetAddrFromEntry() -- From an entry, calculate an address for the next table level
//-------------------------------------------------------------------------------------------------------------------
static struct PageEntry *GetAddrFromEntry(struct PageEntry *entry)
{
    uint64_t frame = entry->frame << 12;
    uint32_t frame32 = (uint32_t)(frame & 0xffffffff);
    return (struct PageEntry *)frame32;
}


//-------------------------------------------------------------------------------------------------------------------
// GetTableEntry() -- From the address, get the table entry shifting properly
//-------------------------------------------------------------------------------------------------------------------
static struct PageEntry *GetTableEntry(struct PageEntry *table, arch_addr_t addr, int shift)
{
    uint32_t index = (addr >> shift) & 0x1ff;
    struct PageEntry *rv = &table[index];

    if (rv->p == 0 && shift != MMU_PT_BITS) {       // we will not allocate a new frame
        uint32_t frame = FrameNew();
        memset((void *)(frame << 12), 0, 4096);
        rv->frame = frame;
        rv->p = 1;
        rv->rw = 1;
        rv->us = 1;
    }

    return rv;
}


//-------------------------------------------------------------------------------------------------------------------
// MmuMapAddress() -- Map an address to a frame: the address is not present and the frame is allocated
//-------------------------------------------------------------------------------------------------------------------
bool MmuMapAddress(arch_addr_t addr, uint32_t frame)
{
#ifdef DEBUG_MMU
    kprintf(u8"MMU: Mapping address 0x%08x %08x to frame %p\n", (uint32_t)(addr >> 32), (uint32_t)addr, frame);
#endif

    if (!IsFrameAlloc(frame)) {
        kprintf(u8"MMU: Frame %p is not allocated\n", frame);
        return false;
    }

    struct PageEntry *pml4Table = cr3;
    struct PageEntry *pml4Entry = GetTableEntry(pml4Table, addr, MMU_PML4_BITS);
    struct PageEntry *pdpTable = GetAddrFromEntry(pml4Entry);
    struct PageEntry *pdpEntry = GetTableEntry(pdpTable, addr, MMU_PDPT_BITS);
    struct PageEntry *pdTable = GetAddrFromEntry(pdpEntry);
    struct PageEntry *pdEntry = GetTableEntry(pdTable, addr, MMU_PD_BITS);
    struct PageEntry *pTable = GetAddrFromEntry(pdEntry);
    struct PageEntry *pEntry = GetTableEntry(pTable, addr, MMU_PT_BITS);

    if (pEntry->p) {
        kprintf(u8"MMU: Address 0x%08x %08x is already mapped to frame %p\n", (uint32_t)(addr >> 32), 
                (uint32_t)addr, pEntry->frame);
        return false;
    }

    pEntry->frame = frame;
    pEntry->p = 1;
    pEntry->rw = 1;
    pEntry->us = 1;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MmuTablesInit() -- Perform the required initialization
//-------------------------------------------------------------------------------------------------------------------
void MmuTablesInit(void)
{
    //
    // -- Start by picking up a leftover PhysMM bit of initialization for this architecture
    //    ---------------------------------------------------------------------------------
    FrameAlloc(0);

    extern char _loaderStart[];
    extern char _loaderEnd[];

    // -- First we need to allocate a frame for the PML4 Table; this will be the physical address to put in CR3.
    //    For this we will take the highest available frame on the system (within the 32-bit address range).
    //    ----------------------------------------------------------------------------------------------------------
    NewPml4Addr();

    // OK, from the EBDA to 1MB, we need to identity-map that memory
    uint32_t frame = (mbLocal.ebda >> 12) & 0xffffffff;
    if (frame == 0) frame = 0xa0000;

    for ( ; frame < 0x100; frame ++) {
        if (!MmuMapAddress(frame << 12, frame)) {
            kprintf(u8"MMU: Failed to identity-map the ebda & upper memory area\n");
        }
    }

    // Next, we identity-map the loader (required when we turn paging on)
    for (frame = ((uint32_t)_loaderStart) >> 12; frame < (((uint32_t)_loaderEnd) >> 12); frame ++) {
        if (!MmuMapAddress(frame << 12, frame)) {
            kprintf(u8"MMU: Failed to identity map the loader\n");
        }
    }

    // take care of the recursive map on the PML4 table    
    struct PageEntry *rEntry = &cr3[511];
    rEntry->frame = (uint32_t)cr3 >> 12;
    rEntry->p = 1;
    rEntry->rw = 1;
    rEntry->us = 1;

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
        uint64_t a = (frame << 12) + MMU_FRAME_BUF;
        uint32_t f = frame + (mbLocal.fbAddr >> 12);
        if (!MmuMapAddress(a, f)) {
            kprintf(u8"MMU: Failed to map the frame buffer\n");
        }
    }

    kprintf(u8"Paging structures initialized\n");

    MmuDumpTables(0xa0000);
    MmuDumpTables(0x100000);
    MmuDumpTables(0xffffffffffffffff);
    MmuDumpTables(MMU_FRAME_BUF);
}


//-------------------------------------------------------------------------------------------------------------------
// MmuDumpTables() -- Dump the MMU Tables for a specific address
//-------------------------------------------------------------------------------------------------------------------
void MmuDumpTables(uint64_t addr)
{
    kprintf(u8"\nMmuDumpTables: Walking the page tables for address 0x%08x %08x\n", (uint32_t)(addr >> 32), 
            (uint32_t)addr);
    kprintf(u8"Level  Address       Index   Frame       us   rw   p\n");
    kprintf(u8"-----  ----------    -----   --------    --   --   -\n");

    uint32_t i = (addr >> MMU_PML4_BITS) & 0x1ff;
    struct PageEntry *w = &cr3[i];

    kprintf(u8"PML4   0x%08x    0x%03x   0x%05x      %u    %u   %u\n", cr3, i, (uint32_t)w->frame, w->us, w->rw, w->p);
    if (!w->p) return;
    struct PageEntry *wt = (struct PageEntry *)(uint32_t)(w->frame << 12);
    i = (addr >> MMU_PDPT_BITS) & 0x1ff;
    w = &wt[i];

    kprintf(u8"PDPT   0x%08x    0x%03x   0x%05x      %u    %u   %u\n", wt, i, (uint32_t)w->frame, w->us, w->rw, w->p);
    if (!w->p) return;
    wt = (struct PageEntry *)(uint32_t)(w->frame << 12);
    i = (addr >> MMU_PD_BITS) & 0x1ff;
    w = &wt[i];

    kprintf(u8"PD     0x%08x    0x%03x   0x%05x      %u    %u   %u\n", wt, i, (uint32_t)w->frame, w->us, w->rw, w->p);
    if (!w->p) return;
    wt = (struct PageEntry *)(uint32_t)(w->frame << 12);
    i = (addr >> MMU_PT_BITS) & 0x1ff;
    w = &wt[i];

    kprintf(u8"PT     0x%08x    0x%03x   0x%05x      %u    %u   %u\n", wt, i, (uint32_t)w->frame, w->us, w->rw, w->p);
}
