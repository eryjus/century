//===================================================================================================================
//
//  loader proto.h -- All function prototypes
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

#ifndef __PROTO_H_INCLUDED__
#define __PROTO_H_INCLUDED__


#include "types.h"


//
// -- We will use a standard prototype without a standard FILE type
//    -------------------------------------------------------------
#ifdef FILE
#undef FILE
#endif
#define FILE void


//-------------------------------------------------------------------------------------------------------------------
// From entry.s
//-------------------------------------------------------------------------------------------------------------------
bool Halt(void);


//-------------------------------------------------------------------------------------------------------------------
// From framebuffer.c (including those from framebuffer-common.c):
//-------------------------------------------------------------------------------------------------------------------
bool FrameBufferInit(void);
void FrameBufferClear(void);
void FrameBufferDrawChar(int ch);
uint16_t FrameBufferSetColor(const char *c);
int putc(int, FILE *);


//-------------------------------------------------------------------------------------------------------------------
// From printf.c
//-------------------------------------------------------------------------------------------------------------------
int kprintf(const char *, ...);
int ksprintf(char *, const char *, ...);
size_t strlen(const char *);

//-------------------------------------------------------------------------------------------------------------------
// From mb1.c (including those included from mb1-common.c)
//-------------------------------------------------------------------------------------------------------------------
void ReadMB1Info(void);


//-------------------------------------------------------------------------------------------------------------------
// From mb2.c (including those included from mb2-common.c)
//-------------------------------------------------------------------------------------------------------------------
void ReadMB2Info(void);


//-------------------------------------------------------------------------------------------------------------------
// Prototypes that are common across all architectures, but implemented architecture-specific
//-------------------------------------------------------------------------------------------------------------------
const char *GetArch(void);




void UartDevInit(void);
void UartPutC(const unsigned char byte);
unsigned char UartGetC(void);
void UartWriteBuf(const unsigned char *buffer, size_t size);
void UartPutS(const char *str);


//-------------------------------------------------------------------------------------------------------------------
// Include architecture-specific prototypes
//-------------------------------------------------------------------------------------------------------------------
#include "arch-proto.h"


#endif