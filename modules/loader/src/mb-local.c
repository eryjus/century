//===================================================================================================================
//
//  loader mb-local.c -- The implementation of the locally formatted MB info
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


#include "types.h"
#include "proto.h"
#include "mb-local.h"


struct MbLocal mbLocal;                 // this will be on the bss, so should be initialized to 0


//-------------------------------------------------------------------------------------------------------------------
// MbLocalSetCmdLine() -- Set the command line parameters from Multiboot (256 bytes max)
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalSetCmdLine(const utf8_t *c)
{
    if (mbLocal.hasCommandLine) return false;
    if (!c) return false;
    if (*c == '\0') return false;
    kstrncpy((char *)mbLocal.cmdLine, (const char *)c, MAX_MB_STRING);
    mbLocal.hasCommandLine = true;
    if (strlen(c) > MAX_MB_STRING) return false;
    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MbLocalSetBootDev() -- Set the boot device hint data
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalSetBootDev(uint32_t d, uint32_t p1, uint32_t p2, uint32_t p3)
{
    if (mbLocal.hasBootDev) return false;
    mbLocal.biosDev = d;
    mbLocal.part1 = p1;
    mbLocal.part2 = p2;
    mbLocal.part3 = p3;
    return true;    
}


//-------------------------------------------------------------------------------------------------------------------
// MbLocalBootloader() -- Set the bootloader from Multiboot (256 bytes max)
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalSetBootloader(const utf8_t *l)
{
    if (mbLocal.hasBootloader) return false;
    if (!l) return false;
    if (*l == '\0') return false;
    kstrncpy(mbLocal.bootloader, l, MAX_MB_STRING);
    mbLocal.hasBootloader = true;
    if (strlen(l) > MAX_MB_STRING) return false;
    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MbLocalAddModule() -- Add a boot loader module to the list of loaded files
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalAddModule(arch_addr_t s, arch_addr_t e, utf8_t *n)
{
    if (mbLocal.numModulesLoaded >= MAX_MB_MODULES) return false;

    mbLocal.modules[mbLocal.numModulesLoaded].modStart = s;
    mbLocal.modules[mbLocal.numModulesLoaded].modEnd = e;
    if (n) kstrncpy((char *)mbLocal.modules[mbLocal.numModulesLoaded].modName, (char *)n, MAX_MB_STRING);
    mbLocal.hasModulesLoaded = true;
    mbLocal.numModulesLoaded ++;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MbLocalBasicMem() -- Set the basic memory parameters
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalBasicMem(uint32_t l, uint32_t u)
{
    if (mbLocal.hasBasicMemory) return false;

    mbLocal.memLower = l;
    mbLocal.memUpper = u;
    mbLocal.hasBasicMemory = true;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// MbLocalAddMmapEntry() -- Add a Memory Map Entry
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalAddMmapEntry(uint64_t a, uint64_t l, int t)
{
    if (mbLocal.numMmapEntries >= MAX_MB_MEM) return false;

    mbLocal.mmap[mbLocal.numMmapEntries].baseAddr = a;
    mbLocal.mmap[mbLocal.numMmapEntries].length = l;
    mbLocal.mmap[mbLocal.numMmapEntries].type = t;
    mbLocal.hasMmap = true;
    mbLocal.numMmapEntries ++;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// mbLocalSetVbe() -- Set the VBE information
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalSetVbe(uint16_t m, uint16_t s, uint16_t o, uint16_t l, uint8_t *c, uint8_t *i)
{
    if (mbLocal.hasVbeInfo) return false;

    mbLocal.vbeMode  = m;
    mbLocal.vbeIfaceSeg  = s;
    mbLocal.vbeIfaceOff  = o;
    mbLocal.vbeIfaceLen  = l;
    memmove(mbLocal.vbeControlInfo, c, 512);
    memmove(mbLocal.vbeModeInfo, i, 256);
    mbLocal.hasVbeInfo = true;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// mbLocalSetFb() -- Set the framebuffer data
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalSetFb(uint64_t a, uint32_t p, uint32_t w, uint32_t h, uint8_t b, uint8_t t)
{
    if (mbLocal.hasFrameBuffer) return false;

    mbLocal.fbAddr  = a;
    mbLocal.fbPitch  = p;
    mbLocal.fbWidth  = w;
    mbLocal.fbHeight  = h;
    mbLocal.fbBpp  = b;
    mbLocal.fbType  = t;
    mbLocal.hasFrameBuffer = true;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// mbLocalAddPalletColor() -- Add a color to the color pallet; requires fb & fbType == 0
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalAddPalletColor(uint8_t r, uint8_t g, uint8_t b)
{
    if (!mbLocal.hasFrameBuffer) return false;
    if (mbLocal.fbType != 0) return false;
    if (mbLocal.palletColors >= MAX_MB_PALLET) return false;

    mbLocal.colors[mbLocal.palletColors].red = r;
    mbLocal.colors[mbLocal.palletColors].green = g;
    mbLocal.colors[mbLocal.palletColors].blue = b;
    mbLocal.palletColors ++;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// mbLocalSetElfSyms() -- Set ELF Symbol tables
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalSetElfSyms(uint16_t n, uint16_t s, uint16_t x)
{
    if (mbLocal.hasElf) return false;

    mbLocal.elfNum = n;
    mbLocal.elfEntrySize = s;
    mbLocal.elfShndx = x;
    mbLocal.hasElf = true;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// mbLocalAddElfHdr() -- Add an elf header to the works
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalAddElfHdr(void)
{
    kprintf(u8"TODO: Implement adding an ELF header pointer\n");
    return true;
}


//-------------------------------------------------------------------------------------------------------------------
// mbLocalSetApm() -- Set Advanced Power Management tables
//-------------------------------------------------------------------------------------------------------------------
bool MbLocalSetApm(uint16_t v, uint16_t s, uint32_t o, uint16_t c16, uint16_t d, uint16_t f, uint16_t cl, 
        uint16_t c16l, uint16_t dl)
{
    if (mbLocal.hasApm) return false;

    mbLocal.apmVersion = v;
    mbLocal.apmCSeg = s;
    mbLocal.apmOffset = o;
    mbLocal.apmCSeg16 = c16;
    mbLocal.apmDSeg = d;
    mbLocal.apmFlags = f;
    mbLocal.apmCSegLen = cl;
    mbLocal.apmCSeg16Len = c16l;
    mbLocal.apmDSegLen = dl;
    mbLocal.hasApm = true;

    return true;    
}

