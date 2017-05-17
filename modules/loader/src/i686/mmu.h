//===================================================================================================================
//
//  loader i686/mmu.h -- The structures for interfacing with the Memory Management Unit (MMU)
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//  PAE is not going to be supported.  This is going to be considered another architecture if it is supported.
//  This makes sense since the address space would change from 32-bits to 36-bits, and therefore requiring 64-bits 
//  to store an address.  This requires a cross-compiler different from the i686-elf to do.
//
//  So, with the 32-bit address space, we have a 32-bit entry that will reference a 4K page at the Page Table
//  level.  Therefore, we can have 1024 entries per Page Table, or 4MB per Page Directory Entry.  The Page
//  Directory has 1024 entries, resulting in 4GB addressable space.  It is also important to note that with 
//  recursive mapping, we give up 4MB of virtual address space.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-05-14  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __MMU_H_INCLUDED__
#define __MMU_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


//-------------------------------------------------------------------------------------------------------------------
// This is a 32-bit page entry for both the page directory and the page tables
//-------------------------------------------------------------------------------------------------------------------
struct PageEntry {
    unsigned int p : 1;                 // Is the page present?
    unsigned int rw : 1;                // set to 1 to allow writes
    unsigned int us : 1;                // 0=Supervisor; 1=user
    unsigned int pwt : 1;               // Page Write Through
    unsigned int pcd : 1;               // Page-level cache disable
    unsigned int a : 1;                 // accessed
    unsigned int d : 1;                 // dirty (needs to be written for a swap)
    unsigned int pat : 1;               // set to 0 for tables, page Page Attribute Table (set to 0)
    unsigned int g : 1;                 // Global (set to 0)
    unsigned int avl : 3;               // Available for software use
    unsigned int frame : 20;            // This is the 4K aligned page frame address (or table address)
} __attribute__((packed));

 
#endif
