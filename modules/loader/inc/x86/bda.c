//===================================================================================================================
//
//  loader inc/x86/bda.c -- x86 common architecture BDA functions
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
//  2017-05-03  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"
#include "mb-local.h"


//-------------------------------------------------------------------------------------------------------------------
// ReadBda() -- Read the x86 BIOS Data Area
//-------------------------------------------------------------------------------------------------------------------
void ReadBda(void)
{
    uint16_t u16;

    // -- COM1?
    u16 = *(uint16_t *)(BDA + BDA_COM1);
    if (u16 != 0) MbLocalAddSerial(HW_PORT, u16);

    // -- COM2?
    u16 = *(uint16_t *)(BDA + BDA_COM2);
    if (u16 != 0) MbLocalAddSerial(HW_PORT, u16);

    // -- COM3?
    u16 = *(uint16_t *)(BDA + BDA_COM3);
    if (u16 != 0) MbLocalAddSerial(HW_PORT, u16);

    // -- COM4?
    u16 = *(uint16_t *)(BDA + BDA_COM4);
    if (u16 != 0) MbLocalAddSerial(HW_PORT, u16);

    // -- LPT1?
    u16 = *(uint16_t *)(BDA + BDA_LPT1);
    if (u16 != 0) MbLocalAddParallel(HW_PORT, u16);

    // -- LPT2?
    u16 = *(uint16_t *)(BDA + BDA_LPT2);
    if (u16 != 0) MbLocalAddParallel(HW_PORT, u16);

    // -- LPT3?
    u16 = *(uint16_t *)(BDA + BDA_LPT3);
    if (u16 != 0) MbLocalAddParallel(HW_PORT, u16);

    // -- EBDA
    u16 = *(uint16_t *)(BDA + BDA_EBDA);
    if (u16 != 0) MbLocalSetEbda(u16 << 4);

    // -- Video
    u16 = *(uint16_t *)(BDA + BDA_VIDEO);
    if (u16 != 0) MbLocalAddVideo(HW_PORT, u16);
}
