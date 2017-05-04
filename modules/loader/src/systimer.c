//===================================================================================================================
//
//  kernel systimer.c -- This is the basic interface to the system timer (for more accurate timing)
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//  TODO: When QEMU has addressed the missing System Timer, address the hack in BusyWait.  The progrss of the QEMU
//        team resolving this issue can be tracked at https://bugs.launchpad.net/qemu/+bug/1680991
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-04  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"

extern uint64_t ReadSysTimerCount(void);


//-------------------------------------------------------------------------------------------------------------------
// BusyWait() -- Wait a some number of MICRO-seconds
//
// This is 1 MILLIONTH of a second, not 1 THOUSANDTH of a second.  Be aware of the values you pass in here. 
//
// FIXME: This function includes a hack to get around QEMU which does not currently implement the System Timer 
//        at 0x3f003000.  When the starting ticks come back as 0, we assume that we need to take a less accurate
//        method.
//-------------------------------------------------------------------------------------------------------------------
void BusyWait(uint32_t microSecs)
{
    volatile uint64_t start = ReadSysTimerCount();      // use volatile to prevent the compiler from optimizing away
    uint64_t target = start + microSecs;

    if (start == 0) while (start) --start;
    else while (ReadSysTimerCount() < target) {}
}
