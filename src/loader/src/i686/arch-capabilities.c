//===================================================================================================================
//
//  loader i686/arch-capabilities.c -- Build the Architecture-specific capabilities structures for i686
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
//  2017-04-27  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "capabilities.h"
#include "proto.h"
#include "hw.h"



//-------------------------------------------------------------------------------------------------------------------
// ArchCapabilities() -- i686 capabilities
//-------------------------------------------------------------------------------------------------------------------
void CapabilitiesArch(void)
{
    uint16_t u16;

    //
    // -- start with the BDA
    //    ------------------

    // -- COM1?
    u16 = *(uint16_t *)(BDA + BDA_COM1);
    if (u16 != 0) CapabilitiesUart("COM1", false, u16);

    // -- COM2?
    u16 = *(uint16_t *)(BDA + BDA_COM2);
    if (u16 != 0) CapabilitiesUart("COM2", false, u16);

    // -- COM3?
    u16 = *(uint16_t *)(BDA + BDA_COM3);
    if (u16 != 0) CapabilitiesUart("COM3", false, u16);

    // -- COM4?
    u16 = *(uint16_t *)(BDA + BDA_COM4);
    if (u16 != 0) CapabilitiesUart("COM4", false, u16);

    // -- LPT1?
    u16 = *(uint16_t *)(BDA + BDA_LPT1);
    if (u16 != 0) CapabilitiesParallel("LPT1", false, u16);

    // -- LPT2?
    u16 = *(uint16_t *)(BDA + BDA_LPT2);
    if (u16 != 0) CapabilitiesParallel("LPT2", false, u16);

    // -- LPT3?
    u16 = *(uint16_t *)(BDA + BDA_LPT3);
    if (u16 != 0) CapabilitiesParallel("LPT3", false, u16);

    // -- EBDA
    u16 = *(uint16_t *)(BDA + BDA_EBDA);
    if (u16 != 0) CapabilitiesEbda(u16 << 4);

    // -- Video
    u16 = *(uint16_t *)(BDA + BDA_VIDEO);
    if (u16 != 0) CapabilitiesVideo("Video", false, u16);
}

