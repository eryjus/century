//===================================================================================================================
//
//  capabilities.h -- The structure definitions needed to enumerate the capabilities of the system
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//  So, these definitions will be a bit of a challenge to implement.  They need to be able to represent hardware
//  across all architectures.  The goal will be to pass the address of this structure to the kernel (or have a 
//  predefined location) so that all the messiness of discovery is hidden from the kernel.  The kernel will be
//  responsible for loading the proper drivers based on the capabilities passed on by the loader.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-26  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __CAPABILITIES_H_INCLUDED__
#define __CAPABILITIES_H_INCLUDED__


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "hw.h"
#include "types.h"


#define NAME_LEN        32


//
// -- This enumeration identifies the different capability types that can be discovered on the system
//    -----------------------------------------------------------------------------------------------
enum CapType {
    CAP_SYSTEM_MASTER,
    CAP_UART,
    CAP_PARALLEL,
    CAP_EBDA,
    CAP_VIDEO,
};


//
// -- This structure defines a UART capability
//    ----------------------------------------
struct UartCapability {
    bool memMappedIO;                   // if not, then hardware port
    addr_t baseAddr;
};


//
// -- This structure defines a Parallel port
//    --------------------------------------
struct ParallelCapability {
    bool memMappedIO;                   // if not, then hardware port
    addr_t baseAddr;
};


//
// -- This structure defines the Extended BIOS Data Area
//    --------------------------------------------------
struct Ebda {
    addr_t baseAddr;
};


//
// -- This structure defines a Video card
//    -----------------------------------
struct VideoCapability {
    bool memMappedIO;                   // if not, then hardware port
    addr_t baseAddr;
};


//
// -- This is the basic capability definition, which includes a union of the plethora of capability types
//    ---------------------------------------------------------------------------------------------------
struct Capability {
    enum CapType type;
    uint32_t parent;                    // this is the parent capability, 0 is at the highest level
    char name[NAME_LEN + 1];
    union {
        struct UartCapability uart;
        struct ParallelCapability parallel;
        struct Ebda ebda;
    };
};


//
// -- Start with the capabilities header, which will describe the size of the capabilities table
//    ------------------------------------------------------------------------------------------
struct Capabilities {
    uint32_t count;                     // this is the number of capability nodes in this table
    struct Capability cap[0];           // this is the system master capability
};





#endif
