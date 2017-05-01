//===================================================================================================================
//
//  loader mb2.h -- These are the structures for the multiboot 2 info block
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
//  2017-04-29  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __MB2_H_INCLUDED__
#define __MB2_H_INCLUDED__


#include "types.h"


//
// -- The fixed multiboot info structure elements
//    -------------------------------------------
struct Mb2Fixed {
    uint32_t totalSize;
    uint32_t reserved;
} __attribute__((packed));


//
// -- This is the basic tag header information -- every tag has one
//    -------------------------------------------------------------
struct Mb2BasicTag {
    uint32_t type;              
    uint32_t size;              
} __attribute__((packed));


//
// -- Basic memory info
//    -----------------
struct Mb2BasicMem {
    struct Mb2BasicTag tag;         // type == 4; size == 16
    uint32_t memLower;
    uint32_t memUpper;
} __attribute__((packed));


//
// -- Boot device information
//    -----------------------
struct Mb2BootDevice{
    struct Mb2BasicTag tag;         // type == 5; size == 20
    uint32_t biosDev;
    uint32_t partition;
    uint32_t subPartition;
} __attribute__((packed));


//
// -- The command line for the boot
//    -----------------------------
struct Mb2CmdLine {
    struct Mb2BasicTag tag;         // type == 1
    uint8_t cmdLine[0];
} __attribute__((packed));


//
// -- A laoded module
//    ---------------
struct Mb2Module {
    struct Mb2BasicTag tag;         // type == 3
    uint32_t modStart;
    uint32_t modEnd;
    uint8_t name[0];
} __attribute__((packed));


//
// -- THe ELF Symbols
//    ---------------
struct Mb2ElfSymbols {
    struct Mb2BasicTag tag;         // type == 9
    uint16_t num;
    uint16_t intSize;
    uint16_t shndx;
    uint16_t reserved;
    uint8_t sectionHdrs[0];
} __attribute__((packed));


//
// -- Memory Map
//    ----------
struct Mb2MemMap {
    struct Mb2BasicTag tag;         // type == 6
    uint32_t entrySize;
    uint32_t entryVersion;
    struct {
        uint64_t baseAddr;
        uint64_t length;
        uint32_t type;
        uint32_t reserved;
    } entries [0];
} __attribute__((packed));


//
// -- The bootloader name
//    -------------------
struct Mb2Loader {
    struct Mb2BasicTag tag;         // type == 2
    uint8_t name[0];
} __attribute__((packed));


//
// -- The APM Table
//    -------------
struct Mb2Apm {
    struct Mb2BasicTag tag;         // type == 10; size == 28
    uint16_t version;
    uint16_t cseg;
    uint32_t offset;
    uint16_t cseg16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t csegLen;
    uint16_t cseg16Len;
    uint16_t dsegLen;
} __attribute__((packed));


//
// -- The VBE Table
//    -------------
struct Mb2VbeInfo {
    struct Mb2BasicTag tag;         // type == 7; size == 784
    uint16_t vbeMode;
    uint16_t vbeInterfaceSeg;
    uint16_t vbeInterfaceOff;
    uint16_t vbeInterfaceLen;
    uint8_t vbeControlInfo[512];
    uint8_t vbeModeInfo[256];
} __attribute__((packed));


//
// -- The FrameBuffer Info
//    -------------
struct Mb2FbInfo {
    struct Mb2BasicTag tag;         // type == 8
    uint64_t fbAddr;
    uint32_t fbPitch;
    uint32_t fbWidth;
    uint32_t fbHeight;
    uint8_t fbBpp;
    uint8_t fbType;
    union {
        struct fbPallet {       // when fbType == 0
            uint32_t palletColors;
            struct {
                uint8_t red;
                uint8_t green;
                uint8_t blue;
            } color [0];
        } pallet;
        struct fbRgb {          // when fbType == 1
            uint8_t redFieldPos;
            uint8_t redMaskSize;
            uint8_t greenFieldPos;
            uint8_t greenMaskSize;
            uint8_t blueFieldPos;
            uint8_t blueMaskSize;
        } rgb;
    };
} __attribute__((packed));


extern struct Mb2Fixed *mb2Data;


#endif
