//===================================================================================================================
//
//  loader mb1.h -- These are the structures for the multiboot 1 info block
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
//  2017-04-18  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __MB1_H_INCLUDED__
#define __MB1_H_INCLUDED__


#include "types.h"


//
// -- This is the loaded modules block (which will repeat)
//    ----------------------------------------------------
struct Mb1Mods {
    uint32_t modStart;
    uint32_t modEnd;
    const char *modIdent;
    uint32_t modReserved;
} __attribute__((packed));


//
// -- Memory Map entries, which will repeat (pointer points to mmapAddr)
//    ------------------------------------------------------------------
struct Mb1MmapEntry {
    uint32_t mmapSize;
    uint64_t mmapAddr;
    uint64_t mmapLength;
    uint32_t mmapType;
} __attribute__((packed));


//
// -- The drives on the system
//    ------------------------
struct Mb1Drives {
    uint32_t drivesSize;
    uint8_t driveNumber;
    uint8_t driveMode;
    uint16_t driveCyl;
    uint8_t driveHeads;
    uint8_t driveSectors;
    uint16_t drivePorts[0];
} __attribute__((packed));


//
// -- The Advanced Power Management BIOS table info
//    ---------------------------------------------
struct Mb1ApmTable {
    uint16_t apmVersion;
    uint16_t apmCSeg;
    uint32_t apmOffset;
    uint16_t apmCSeg16;
    uint16_t apmDSeg;
    uint16_t apmFlags;
    uint16_t apmCSegLen;
    uint16_t apmCSeg16Len;
    uint16_t apmDSegLen;
} __attribute__((packed));


//
// -- Multiboot Pallet Information
//    ----------------------------
struct MultibootPallet {
    uint32_t fbPalletAddr;
    uint16_t fbPalletNumColors;
} __attribute__((packed));


//
// -- Multiboot pallet rgb setting
//    ----------------------------
struct MultibootColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} __attribute__((packed));


//
// -- MultibootRgb
//    ------------
struct MultibootRbg {
    uint8_t fbRedFieldPosition;
    uint8_t fbRedMaskSize;
    uint8_t fbGreenFieldPosition;
    uint8_t fbGreenMaskSize;
    uint8_t fbBlueFieldPosition;
    uint8_t fbBlueMaskSize;
} __attribute__((packed));


//
// -- The Multiboot 1 Info block
//    --------------------------
struct Multiboot1Info {
    uint32_t flags;                         // flags of what is available
    uint32_t memLower;       
    uint32_t memUpper;
    uint32_t bootDevice;
    const char *cmdLine;
    uint32_t moduleCount;
    struct Mb1Mods *modules;
    uint32_t symsTabSize;
    uint32_t symsStrSize;
    uint32_t symsAddr;
    uint32_t symsReserved;
    uint32_t mmapLength;
    struct Mb1MmapEntry *mmap;
    uint32_t drivesLength;
    struct Mb1Drives *drives;
    void *configTable;                      // The ROM configuration table
    const char *bootLoaderName;
    struct Mb1ApmTable *apmTable;
    uint32_t vbeControlInfo;
    uint32_t vbeModeInfo;
    uint16_t vbeMode;
    uint16_t vbeInterfaceSeg;
    uint16_t vbeInterfaceOff;
    uint16_t vbeInterfaceLen;
    uint64_t fbAddr;
    uint32_t fbPitch;
    uint32_t fbWidth;
    uint32_t fbHeight;
    uint8_t fbBpp;
    uint8_t fbType;
    union {
        struct MultibootPallet pallet;
        struct MultibootRbg rbg;
    };
} __attribute__((packed));


extern struct Multiboot1Info *mb1Data;


#endif
