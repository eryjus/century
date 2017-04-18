//===================================================================================================================
//
//  loader rpi2b/hw.h -- Hardware locations
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
//  2017-04-17  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __HW_H_INCLUDED__
#define __HW_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


//
// -- architecture-specific type sizes
//    --------------------------------
typedef uint32_t addr_t;                    // This is the size of an address in this architecture


//-------------------------------------------------------------------------------------------------------------------
// Mailboxes
//-------------------------------------------------------------------------------------------------------------------


//
// -- The hardware base address
//    -------------------------
#define HW_BASE             (0x3f000000)                // this is the base address for all hardware I/O addresses


//
// -- The base mailbox address
//    ------------------------
#define MB_BASE             (HW_BASE + 0xb880)          // the base address for all mailbox operations


//
// -- Some mailbox address offsets
//    ----------------------------
#define MB_READ             (0x00)                      // MB: Receiving mail
#define MB_POLL             (0x10)                      // MB: Read witout receiving
#define MB_SENDER           (0x14)                      // MB: Sender information
#define MB_STATUS           (0x18)                      // MB: Information
#define MB_CONFIG           (0x1c)                      // MB: Settings
#define MB_WRITE            (0x20)                      // MB: Send mail


#endif
