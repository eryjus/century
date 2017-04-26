//===================================================================================================================
//
//  loader x86_64/proto.h -- Architecture specific prototypes
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
//  2017-04-19  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __ARCH_PROTO_H_INCLUDED__
#define __ARCH_PROTO_H_INCLUDED__


//
// -- This is a dummy function to return 0 for the system timer; calling function can handle
//    --------------------------------------------------------------------------------------
inline static uint64_t ReadSysTimerCount(void) { return 0; }


//
// -- Read/Write to hardware port
//    ---------------------------
uint8_t inb(uint16_t port);
uint8_t outb(uint16_t port, uint8_t byte);


#endif
