//===================================================================================================================
//
//  kernel proto.h -- This file aggregates all the function prototypes available to the kernel.
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
//  2017-04-05  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __PROTO_H_INCLUDED__
#define __PROTO_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "hwtypes.h"


//-------------------------------------------------------------------------------------------------------------------
// kernel/src/rpi2b/lowsystimer.s
//-------------------------------------------------------------------------------------------------------------------
uint64_t ReadSysTimerCount(void);


//-------------------------------------------------------------------------------------------------------------------
// kernel/src/rpi2b/mmio.c
//-------------------------------------------------------------------------------------------------------------------
void WriteMmio(addr_t reg, uint32_t data);
uint32_t ReadMmio(addr_t reg);


//-------------------------------------------------------------------------------------------------------------------
// kernel/src/rpi2b/systimer.c
//-------------------------------------------------------------------------------------------------------------------
void BusyWait(uint32_t microSecs);


//-------------------------------------------------------------------------------------------------------------------
// kernel/src/rpir2b/uart-dev.c
//-------------------------------------------------------------------------------------------------------------------
extern struct serial_device uart1;

void UartDevInit(void);

unsigned char UartGetC(void);
void UartPutC(const unsigned char byte);
void UartPutS(const char *str);
void UartWriteBuf(const unsigned char *buffer, size_t size);


//-------------------------------------------------------------------------------------------------------------------
// kernel/src/string.c
//-------------------------------------------------------------------------------------------------------------------
size_t strlen(const char *str);


#endif