//===================================================================================================================
//
//  loader mb-local.h -- Locally formatted copies of the multiboot information
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
//  The purpose of these structures is to copy the data from the first MB to this location so that the lower
//  1MB of memory can be effectively freed up.  We will use the lower 1MB of memory ourselves to communicate 
//  to the kernel.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-29  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __MB_LOCAL_H_LOADED__
#define __MB_LOCAL_H_LOADED__

#include "types.h"


//
// -- These defines identify some of the limitations of the stage-3 loader
//    --------------------------------------------------------------------
#define MAX_MB_STRING   (512 - 1)
#define MAX_MB_MODULES  (32)
#define MAX_MB_MEM      (32)
#define MAX_MB_PALLET   (256)
#define MAX_MB_SERIAL   (8)
#define MAX_MB_PARALLEL (4)
#define MAX_MB_VIDEO    (1)


//
// -- These are the memory types known
//    --------------------------------
enum MmapType {
    MMAP_RESERVED = 0,
    MMAP_FREE = 1,
    MMAP_ACPI = 3,
    MMAP_PRESERVE = 4,
};


//
// -- Different architectures have different hardware interface types
//    ---------------------------------------------------------------
enum IfaceType {
    HW_PORT = 0x01,
    HW_MMIO = 0x02,
    HW_MBOX = 0x03,
};


//
// -- This structure will hold a stage-3 loader local copy of the provided multiboot information.
//    -------------------------------------------------------------------------------------------
struct MbLocal {
    // -- The command line parameters (max length 512 bytes, including terminating NULL)
    bool hasCommandLine;
    utf8_t cmdLine[MAX_MB_STRING + 1];

    // -- The boot loader name that got us here (max length 512 bytes, including terminating NULL)
    bool hasBootloader;
    utf8_t bootloader[MAX_MB_STRING + 1];

    // -- The modules loaded
    bool hasModulesLoaded;
    uint32_t numModulesLoaded;
    struct {
        arch_addr_t modStart;
        arch_addr_t modEnd;
        utf8_t modName[MAX_MB_STRING + 1];
    } modules[MAX_MB_MODULES];

    // -- Basic memory sizes
    bool hasBasicMemory;
    uint32_t memLower;
    uint32_t memUpper;

    // -- Boot device clues
    bool hasBootDev;
    uint32_t biosDev;
    uint32_t part1;
    uint32_t part2;
    uint32_t part3;

    // -- Memory Map Information
    bool hasMmap;
    uint32_t numMmapEntries;
    struct {
        uint64_t baseAddr;
        uint64_t length;
        enum MmapType type;
    } mmap[MAX_MB_MEM];

    // -- VBE Information
    bool hasVbeInfo;
    uint16_t vbeMode;
    uint16_t vbeIfaceSeg;
    uint16_t vbeIfaceOff;
    uint16_t vbeIfaceLen;
    uint8_t vbeControlInfo[512];
    uint8_t vbeModeInfo[256];

    // -- Frame Buffer Info
    bool hasFrameBuffer;
    uint64_t fbAddr;
    uint32_t fbPitch;
    uint32_t fbWidth;
    uint32_t fbHeight;
    uint8_t fbBpp;
    uint8_t fbType;         // 0 == use color pallet; 1 == direct RGB; 2 == EGA text (lines/cols rather than pixels)

    // -- color pallet info when fbType == 0
    uint32_t palletColors;
    struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } colors[MAX_MB_PALLET];

    // -- ELF debugging symbols
    bool hasElf;
    uint16_t elfNum;
    uint16_t elfEntrySize;
    uint16_t elfShndx;
    uint8_t elfSectionHdrs[0];              // TODO: replace this with the proper ELF section header struct

    // -- APM information
    bool hasApm;
    uint16_t apmVersion;
    uint16_t apmCSeg;
    uint32_t apmOffset;
    uint16_t apmCSeg16;
    uint16_t apmDSeg;
    uint16_t apmFlags;
    uint16_t apmCSegLen;
    uint16_t apmCSeg16Len;
    uint16_t apmDSegLen;

    // -- Serial Ports
    uint32_t serialPorts;
    struct {
        enum IfaceType type;
        uint32_t locn;
    } serial[MAX_MB_SERIAL];

    // -- Parallel Ports
    uint32_t parallelPorts;
    struct {
        enum IfaceType type;
        uint32_t locn;
    } parallel[MAX_MB_PARALLEL];

    // -- Video Interfaces
    uint32_t videoInterfaces;
    struct {
        enum IfaceType type;
        uint32_t locn;
    } video[MAX_MB_VIDEO];

    uint32_t ebda;


    //
    // -- These additional fields are used to communicate additional information to the kernel 
    //    when we pass control over
    //    ------------------------------------------------------------------------------------
    uint16_t color;
    uint16_t bgColor;
    uint32_t row;
    uint32_t col;

    // -- phys memory manager
    uint64_t memAmount;
    uint32_t *memBitMap;            // this is an external structure that will moved;managed separately
    uint32_t memSizeFrames;         // this is the size of the bitmap in frames

    // -- virtual memory tables address
    arch_addr_t mmu;
};


//
// -- declare the variable globally for the references that will follow
//    -----------------------------------------------------------------
extern struct MbLocal mbLocal;


//
// -- finally, several inline access functions for several fields
//    -----------------------------------------------------------
static inline int16_t GetColor(void) { return mbLocal.color; }
static inline void SetColor(uint16_t c) { mbLocal.color = c; }
static inline int16_t GetBgColor(void) { return mbLocal.bgColor; }
static inline void SetBgColor(uint16_t c) { mbLocal.bgColor = c; }
static inline int32_t GetRow(void) { return mbLocal.row; }
static inline void SetRow(uint32_t r) { mbLocal.row = r; }
static inline int32_t GetCol(void) { return mbLocal.col; }
static inline void SetCol(uint32_t c) { mbLocal.col = c; }
static inline addr_t GetFbAddr(void) { return (addr_t)mbLocal.fbAddr; }
static inline uint32_t GetFbPitch(void) { return mbLocal.fbPitch; }
static inline uint32_t GetFbWidth(void) { return mbLocal.fbWidth; }
static inline uint32_t GetFbHeight(void) { return mbLocal.fbHeight; }
static inline uint8_t GetFbBpp(void) { return mbLocal.fbBpp; }
static inline uint64_t GetMemAmount(void) { return mbLocal.memAmount; }
static inline void SetMemAmount(uint64_t a) { mbLocal.memAmount = a; }
static inline uint32_t GetEbda(void) { return mbLocal.ebda; }


#endif
