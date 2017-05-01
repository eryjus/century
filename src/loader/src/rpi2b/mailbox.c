//===================================================================================================================
//
//  loader rpi2b/mailbox.c -- This is the interface into the mailboxes
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


#include "types.h"
#include "proto.h"
#include "mmio.h"


//-------------------------------------------------------------------------------------------------------------------
// MailboxWaitReadyToSend() -- Busy-wait for the mailbox system to be ready to send
//-------------------------------------------------------------------------------------------------------------------
static void MailboxWaitReadyToSend(void)
{
    while (MmioRead(MB_BASE + MB_STATUS) & (1 << 31)) { }
}


//-------------------------------------------------------------------------------------------------------------------
// MailboxWaitReadyToReceive() -- Busy-wait for the mailbox system to be ready to receive data
//-------------------------------------------------------------------------------------------------------------------
static void MailboxWaitReadyToReceive(void)
{
    while (MmioRead(MB_BASE + MB_STATUS) & (1 << 30)) { }
}


//-------------------------------------------------------------------------------------------------------------------
// MailboxSend() -- Send a message to the mailbox
//-------------------------------------------------------------------------------------------------------------------
bool MailboxSend(uint32_t message, uint32_t mailbox)
{
    if ((message & 0x0f) != 0 || (mailbox & 0xfffffff0) != 0) return false;

    MailboxWaitReadyToSend();
    MmioWrite(MB_BASE + MB_WRITE, message | mailbox);

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MailboxReceive() -- Receive a message from the mailbox
//-------------------------------------------------------------------------------------------------------------------
uint32_t MailboxReceive(uint32_t mailbox)
{
    if ((mailbox & 0xfffffff0) != 0) return (uint32_t)-1;

    while (true) {
        MailboxWaitReadyToReceive();
        uint32_t msg = MmioRead(MB_BASE + MB_READ);

        if ((msg & 0x0f) == mailbox) return (msg & 0xfffffff0);
    }
}
