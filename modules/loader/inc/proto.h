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


//
// -- We will use a standard prototype without a standard FILE type
//    -------------------------------------------------------------
#ifdef FILE
#undef FILE
#endif
#define FILE void

#include "types.h"


//-------------------------------------------------------------------------------------------------------------------
// Include architecture-specific prototypes
//-------------------------------------------------------------------------------------------------------------------
#include "arch-proto.h"


//-------------------------------------------------------------------------------------------------------------------
// From entry.s
//-------------------------------------------------------------------------------------------------------------------
bool Halt(void);


//-------------------------------------------------------------------------------------------------------------------
// From framebuffer.c (including those from framebuffer-common.c):
//-------------------------------------------------------------------------------------------------------------------
bool FrameBufferInit(void);
void FrameBufferClear(void);
void FrameBufferDrawChar(utf8_t ch);
uint16_t FrameBufferSetColor(const utf8_t *c);
int putc(const utf8_t, FILE *);


//-------------------------------------------------------------------------------------------------------------------
// From printf.c
//-------------------------------------------------------------------------------------------------------------------
int kprintf(const utf8_t *, ...);
int ksprintf(utf8_t *, const utf8_t *, ...);
size_t strlen(const utf8_t *);

//-------------------------------------------------------------------------------------------------------------------
// From mb1.c, mb2.c, and mb-local.c
//-------------------------------------------------------------------------------------------------------------------
void ReadMB1Info(void);
void ReadMB2Info(void);

bool MbLocalSetCmdLine(const utf8_t *c);
bool MbLocalSetBootloader(const utf8_t *l);
bool MbLocalSetBootDev(uint32_t d, uint32_t p1, uint32_t p2, uint32_t p3);
bool MbLocalAddModule(arch_addr_t s, arch_addr_t e, utf8_t *n);
bool MbLocalBasicMem(uint32_t l, uint32_t u);
bool MbLocalAddMmapEntry(uint64_t a, uint64_t l, int t);
bool MbLocalSetVbe(uint16_t m, uint16_t s, uint16_t o, uint16_t l, uint8_t *c, uint8_t *i);
bool MbLocalSetFb(uint64_t a, uint32_t p, uint32_t w, uint32_t h, uint8_t b, uint8_t t);
bool MbLocalAddPalletColor(uint8_t r, uint8_t g, uint8_t b);
bool MbLocalSetElfSyms(uint16_t n, uint16_t s, uint16_t x);
bool MbLocalAddElfHdr(void);
bool MbLocalSetApm(uint16_t v, uint16_t s, uint32_t o, uint16_t c16, uint16_t d, uint16_t f, uint16_t cl, 
        uint16_t c16l, uint16_t dl);
bool MbLocalAddSerial(int t, uint32_t p);
bool MbLocalAddParallel(int t, uint32_t p);
bool MbLocalAddVideo(int t, uint32_t p);
bool MbLocalSetEbda(uint32_t l);


//-------------------------------------------------------------------------------------------------------------------
// From string.c -- functions for string manipulation
//-------------------------------------------------------------------------------------------------------------------
char *kstrncpy(utf8_t *d, const utf8_t *s, size_t n);       // note, copies n characters and the set d[n] to NULL
void memmove(uint8_t *d, const uint8_t *s, size_t cnt);
void memset(void *d, int v, size_t cnt);


//-------------------------------------------------------------------------------------------------------------------
// From mmu.c -- common interface functions for setting up the MMU tables
//-------------------------------------------------------------------------------------------------------------------
void MmuTablesInit(void);
bool MmuMapAddress(arch_addr_t addr, uint32_t frame);
void MmuDumpTables(uint64_t addr);


//-------------------------------------------------------------------------------------------------------------------
// Prototypes that are common across all architectures, but implemented architecture-specific
//-------------------------------------------------------------------------------------------------------------------
const char *GetArch(void);

void UartDevInit(void);
void UartPutC(const utf8_t byte);
unsigned char UartGetC(void);
void UartWriteBuf(const uint8_t *buffer, size_t size);
void UartPutS(const utf8_t *str);


//-------------------------------------------------------------------------------------------------------------------
// From phys-mm.c -- common interface functions for the physical memory structures
//-------------------------------------------------------------------------------------------------------------------
void PhysMMInit(void);
void FrameFree(uint32_t frame);
void FrameAlloc(uint32_t frame);
bool IsFrameAlloc(uint32_t frame);
uint32_t FrameNew(void);
void FrameFreeRange(uint32_t frame, uint32_t len);
void FrameAllocRange(uint32_t frame, uint32_t len); 


#endif