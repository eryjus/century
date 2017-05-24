//===================================================================================================================
//
//  loader x86_64/mmu.h -- The structures for interfacing with the Memory Management Unit (MMU)
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
//  2017-05-22  Initial   0.0.0   ADCL  Initial version
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
    unsigned int pat : 1;               // page address type (varies by table)
    unsigned int g : 1;                 // Global (set to 0)
    unsigned int avl : 3;               // Available for software use
    unsigned long long frame : 36;      // This is the 4K aligned page frame address (or table address)
    unsigned int res : 4;               // Reserved bits (set to 0)
    unsigned int ign : 12;              // Additional ignored bits (available for use)
} __attribute__((packed));

 
#endif
