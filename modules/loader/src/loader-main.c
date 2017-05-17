//===================================================================================================================
//
//  loader laoder-main.c -- The i686 loader main entry point
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
//  2017-04-18  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"


//-------------------------------------------------------------------------------------------------------------------
// DisplayGreeting() -- Display the welcome greeting
//-------------------------------------------------------------------------------------------------------------------
void DisplayGreeting(void)
{
    kprintf(u8"Welcome to Century - %s\n", GetArch());
}


//-------------------------------------------------------------------------------------------------------------------
// LoaderMain() -- this is the main entry point for the loader
//-------------------------------------------------------------------------------------------------------------------
void LoaderMain(void)
{
    UartDevInit();
    UartPutS(u8"Hello!!\n");
    ReadMB1Info();
    ReadMB2Info();
    ReadBda();
    FrameBufferInit();
    DisplayGreeting();
    PhysMMInit();
    MmuTablesInit();
    Halt();
}
