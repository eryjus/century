//===================================================================================================================
//
//  loader i686/mmu.c -- The functions for interfacing with the Memory Management Unit (MMU)
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
//  2017-05-14  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "mmu.h"
#include "proto.h"
#include "mb-local.h"


//-------------------------------------------------------------------------------------------------------------------
// AddressToPde() -- get the address of the Page Directory Entry based on the address we want to manage
//-------------------------------------------------------------------------------------------------------------------
static uint32_t AddressToPde(arch_addr_t virt)
{
#ifdef DEBUG_MMU
    kprintf(u8"MMU: the PDE for %p is 0x%x; ", (uint32_t)virt, (uint32_t)(virt >> 22));
#endif
    return (virt >> 22);
}


//-------------------------------------------------------------------------------------------------------------------
// AddressToPte() -- get the address of the Page Table Entry based on the address we want to manage
//-------------------------------------------------------------------------------------------------------------------
static uint32_t AddressToPte(arch_addr_t virt)
{
#ifdef DEBUG_MMU
    kprintf(u8"MMU: the PTE for %p is 0x%x; ", (uint32_t)virt, ((uint32_t)(virt >> 12)) & 0x3ff);
#endif
    return ((virt >> 12) & 0x3ff);
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

    struct PageEntry *cr3 = (struct PageEntry *)mbLocal.mmu;
    struct PageEntry *pde = (struct PageEntry *)&cr3[AddressToPde(addr)];

    if (!pde->p) {          // allocate a new Page Table... (recursion won't work')
#ifdef DEBUG_MMU
        kprintf(u8"MMU: Creating a new page table in support of address %p\n", addr);
#endif
        arch_addr_t f = FrameNew();
        memset((void *)f, 0, 4096);
        
        pde->frame = f >> 12;
        pde->us = 1;
        pde->rw = 1;
        pde->p = 1;
    }

    struct PageEntry *pte = &((struct PageEntry *)((uint32_t)pde & 0xfffff000))[AddressToPte(addr)];
    if (pte->p) {
        kprintf(u8"MMU: Page already allocated\n");
        return false;               // already allocated!
    }

    pte->frame = frame >> 12;
    pte->us = 1;
    pte->rw = 1;
    pte->p = 1;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MmuTablesInit() -- Initialize the tables for the MMU -- 32-bit i686 tables
//-------------------------------------------------------------------------------------------------------------------
void MmuTablesInit(void)
{
    extern char _loaderStart[];
    extern char _loaderEnd[];

    // -- First we need to allocate a frame for the Page Directory; this will be the physical address to put in CR3.
    //    For this we will take the highest available frame on the system.
    //    ----------------------------------------------------------------------------------------------------------
    FrameAlloc(mbLocal.mmu);
    memset((void *)mbLocal.mmu, 0, 4096);       // -- Clear the frame just to be sure

    if (!MmuMapAddress(0xfffff000, mbLocal.mmu)) {     // -- recursively map the last page
        kprintf(u8"MMU: Recursively mapping Page Directory failed\n");
    }

    // OK, from the EBDA to 1MB, we need to identity-map that memory
    uint32_t frame = (mbLocal.ebda >> 12) & 0xffffffff;

    if (!frame) {
        for ( ; frame < 0x100; frame ++) {
            if (!MmuMapAddress(frame << 12, frame)) {
                kprintf(u8"MMU: Failed to identity-map the ebda & upper memory area\n");
            }
        }
    }

    // Next, we identity-map the loader (required when we turn paging on)
    for (frame = ((uint32_t)_loaderStart) >> 12; frame < (((uint32_t)_loaderEnd) >> 12); frame ++) {
        if (!MmuMapAddress(frame << 12, frame)) {
            kprintf(u8"MMU: Failed to identity map the loader\n");
        }
    }

    // 
    // -- OK, there are a few additional things that will need to be mapped, some of which will be mapped
    //    below and others we will need to map in other sections.  Examples:
    //    * The frame bitmap (which will be relocated before transferring control)
    //    * The kernel space (which should be able to be mapped here, once we have the memory map finalized)
    //    * The additional modules (should those be done in the kernel proper?)
    //    * Map the frame buffer
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
}
