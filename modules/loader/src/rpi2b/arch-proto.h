//===================================================================================================================
//
//  loader rpi2b/proto.h -- Architecture specific prototypes
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
// -- architecture-specific type sizes
//    --------------------------------
typedef uint32_t arch_addr_t;
typedef uint32_t addr_t;                        // This is the size of an address in the current cpu bit model


//-------------------------------------------------------------------------------------------------------------------
// From mailbox.c:
//-------------------------------------------------------------------------------------------------------------------
bool MailboxSend(uint32_t message, uint32_t mailbox);
uint32_t MailboxReceive(uint32_t mailbox);

#endif
