//===================================================================================================================
//
//  loader rpi2b/proto.h -- All function prototypes
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


#include "hw.h"

#include <stdbool.h>


//-------------------------------------------------------------------------------------------------------------------
// From mailbox.c:
//-------------------------------------------------------------------------------------------------------------------
bool MailboxSend(uint32_t message, uint32_t mailbox);
uint32_t MailboxReceive(uint32_t mailbox);


//-------------------------------------------------------------------------------------------------------------------
// From framebuffer.c:
//-------------------------------------------------------------------------------------------------------------------
bool FrameBufferInit(void);
void FrameBufferClear(void);

