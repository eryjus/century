//===================================================================================================================
//
//  loader build-capabilities.c -- These are the actual functions to build the capabilities structures
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
//  2017-04-26  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "capabilities.h"
#include "proto.h"


#define DEBUG_CAPABILITIES

//
// -- This is the global system capability structure
//    ----------------------------------------------
struct Capabilities *capArray = (struct Capabilities *)CAP_LOCATION;


//-------------------------------------------------------------------------------------------------------------------
// CapabilitiesInit() -- Initialize and add the system master capability
//-------------------------------------------------------------------------------------------------------------------
void CapabilitiesInit(void)
{
    capArray->count = 1;
    capArray->cap[0].type = CAP_SYSTEM_MASTER;
    capArray->cap[0].parent = 0;
    kstrncpy(capArray->cap[0].name, "system-master", NAME_LEN);

#ifdef DEBUG_CAPABILITIES
    kprintf("The capabilities structure has been initialized, at address %p\n", capArray);
#endif

    CapabilitiesArch();
}


//-------------------------------------------------------------------------------------------------------------------
// CapabilitiesUart() -- Create a Uart Capability
//-------------------------------------------------------------------------------------------------------------------
void CapabilitiesUart(const char *n, bool mmio, addr_t locn)
{
    capArray->cap[capArray->count].type = CAP_UART;
    capArray->cap[capArray->count].parent = 0;
    kstrncpy(capArray->cap[capArray->count].name, n, NAME_LEN);
    capArray->cap[capArray->count].uart.memMappedIO = mmio;
    capArray->cap[capArray->count].uart.baseAddr = locn;

#ifdef DEBUG_CAPABILITIES
    kprintf("Capability %lu: UART :: %s %s at %p\n", capArray->count, n, mmio?"address":"port", locn);
#endif

    capArray->count ++;
}


//-------------------------------------------------------------------------------------------------------------------
// CapabilitiesParallel() -- Create a Parallel Capability
//-------------------------------------------------------------------------------------------------------------------
void CapabilitiesParallel(const char *n, bool mmio, addr_t locn)
{
    capArray->cap[capArray->count].type = CAP_PARALLEL;
    capArray->cap[capArray->count].parent = 0;
    kstrncpy(capArray->cap[capArray->count].name, n, NAME_LEN);
    capArray->cap[capArray->count].parallel.memMappedIO = mmio;
    capArray->cap[capArray->count].parallel.baseAddr = locn;

#ifdef DEBUG_CAPABILITIES
    kprintf("Capability %lu: Parallel :: %s %s at %p\n", capArray->count, n, mmio?"address":"port", locn);
#endif

    capArray->count ++;
}


//-------------------------------------------------------------------------------------------------------------------
// CapabilitiesEbda() -- Create an Extended BIOS Data Area Location
//-------------------------------------------------------------------------------------------------------------------
void CapabilitiesEbda(addr_t locn)
{
    capArray->cap[capArray->count].type = CAP_EBDA;
    capArray->cap[capArray->count].parent = 0;
    kstrncpy(capArray->cap[capArray->count].name, "EBDA", NAME_LEN);
    capArray->cap[capArray->count].ebda.baseAddr = locn;

#ifdef DEBUG_CAPABILITIES
    kprintf("Capability %lu: EBDA :: %p\n", capArray->count, locn);
#endif

    capArray->count ++;
}


//-------------------------------------------------------------------------------------------------------------------
// CapabilitiesVideo() -- A Video Card
//-------------------------------------------------------------------------------------------------------------------
void CapabilitiesVideo(const char *n, bool mmio, addr_t locn)
{
    capArray->cap[capArray->count].type = CAP_VIDEO;
    capArray->cap[capArray->count].parent = 0;
    kstrncpy(capArray->cap[capArray->count].name, n, NAME_LEN);
    capArray->cap[capArray->count].ebda.baseAddr = locn;

#ifdef DEBUG_CAPABILITIES
    kprintf("Capability %lu: Video :: %s %s at %p\n", capArray->count, n, mmio?"address":"port", locn);
#endif

    capArray->count ++;
}
