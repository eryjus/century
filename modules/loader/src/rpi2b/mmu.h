//===================================================================================================================
//
//  loader rpi2b/mmu.h -- The structures for interfacing with the Memory Management Unit (MMU)
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//  The ARM architecture is different than the x86-family architecture.  The MMU tables are called Translation 
//  Tables and there are 2 Levels: 1 and 2.  There is 1 TTL1 table that is 16K long and up to 4096 TTL2 tables 
//  that are 1K long each.  We are going to stuff 4 X 1K (consecutive) tables into a single 4K frame, mapping all
//  4K as a single operation, and we will aggregate 4 X 4K frames into one 16K aligned TTL1 table.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-05-19  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __MMU_H_INCLUDED__
#define __MMU_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


//-------------------------------------------------------------------------------------------------------------------
// The Translation Table Level 1 structure (TTL1)
//-------------------------------------------------------------------------------------------------------------------
struct Ttl1 {
    unsigned int fault : 2;             // 00=fault; 01=TTL2 table address; 01 and 11 unused
    unsigned int sbz : 3;               // sbz = should be zero
    unsigned int domain : 4;            // domain -- we will use 0b0000 for now
    unsigned int p : 1;                 // unimplemented in the rpi2b arch; use 0
    unsigned int ttl2 : 22;             // the frame address of the ttl2 table (notice aligned to 1K)
} __attribute__((packed));


//-------------------------------------------------------------------------------------------------------------------
// The Translation Table Level 2 structure (TTL2)
//-------------------------------------------------------------------------------------------------------------------
struct Ttl2 {
    unsigned int fault : 2;             // 00=fault; 01=large page(not used); 1x=small page (x sets execute never)
    unsigned int b : 1;                 // buffered
    unsigned int c : 1;                 // cached
    unsigned int ap : 2;                // access permissions
    unsigned int tex : 3;               // Type Extension
    unsigned int apx : 1;               // access permission extension
    unsigned int s : 1;                 // sharable
    unsigned int nG : 1;                // not Global
    unsigned int frame : 20;            // this is the final 4K frame address
} __attribute__((packed));


#endif