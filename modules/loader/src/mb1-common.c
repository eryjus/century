//===================================================================================================================
//
//  loader mb1-common.c -- Multiboot functions that are common to multiple architectures
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


#define DEBUG_MB1


#include "types.h"
#include "proto.h"
#include "mb1.h"
#include "mb-local.h"



//
// -- This is the multiboot 1 information structure (will remain NULL if not booted with MB1-compliant loader)
//    --------------------------------------------------------------------------------------------------------
struct Multiboot1Info *mb1Data = 0;


//-------------------------------------------------------------------------------------------------------------------
// ReadMB1Info() -- Read the multiboot information from the data provided and store it locally
//-------------------------------------------------------------------------------------------------------------------
void ReadMB1Info(void)
{
    if (!mb1Data) return;

#ifdef DEBUG_MB1
    kprintf(u8"Reading Multiboot1 Information (we have a structure at %p)\n", mb1Data);
    kprintf(u8"  The flags are %p\n", mb1Data->flags);
#endif

    if (mb1Data->flags & (1<<0)) {
        MbLocalBasicMem(mb1Data->memLower, mb1Data->memUpper);
    }

    if (mb1Data->flags & (1<<1)) {
        MbLocalSetBootDev(mb1Data->bootDevice & 0xff, (mb1Data->bootDevice >> 8) & 0xff, 
                (mb1Data->bootDevice >> 16) & 0xff, (mb1Data->bootDevice >> 24) & 0xff);
    }

    if (mb1Data->flags & (1<<2)) {
        MbLocalSetCmdLine(mb1Data->cmdLine);
    }

    if (mb1Data->flags & (1<<3)) {
        uint32_t i;
        struct Mb1Mods *m;

        for (m = mb1Data->modules, i = 0; i < mb1Data->moduleCount; i ++) {
            kprintf("Adding module %s\n", m[i].modIdent);
            MbLocalAddModule(m[i].modStart, m[i].modEnd, m[i].modIdent);
        }
    }

    if (mb1Data->flags & (1<<4)) {
        kprintf(u8"TODO: Set the non-ELF Symbols in the MBLocal Structure\n");
    }

    if (mb1Data->flags & (1<<5)) {
        MbLocalSetElfSyms(mb1Data->elf.elfNum, mb1Data->elf.elfSize, mb1Data->elf.elfShndx);
    }

    if (mb1Data->flags & (1<<6)) {
        uint32_t size = mb1Data->mmapLength;
        struct Mb1MmapEntry *entry = mb1Data->mmap;
        while (size) {
            MbLocalAddMmapEntry(entry->mmapAddr, entry->mmapLength, entry->mmapType);
            uint64_t newLimit = entry->mmapAddr + entry->mmapLength;
            if (newLimit > GetMemAmount()) SetMemAmount(newLimit);
            size -= (entry->mmapSize + 4);
            entry = (struct Mb1MmapEntry *)(((uint32_t)entry) + entry->mmapSize + 4);
        }

        kprintf(u8"Found 0x%08lx %08lx bytes of memory\n", (uint32_t)(GetMemAmount() >> 32), (uint32_t)(GetMemAmount() & 0xffffffff));
        uint32_t memSize = (uint32_t)(GetMemAmount() >> 12);
        kprintf(u8"  0x%lx pages; bitmap frames 0x%lx\n", memSize, (memSize >> (12 + 3)) + (memSize&0x7fff?1:0));
    }

    if (mb1Data->flags & (1<<7)) {
        kprintf(u8"TODO: Determine if the Drives data is valid enough to add to MbLocal\n");
    }

    if (mb1Data->flags & (1<<8)) {
        kprintf(u8"TODO: Determine if the Config Table data is valid enough to add to MbLocal\n");
    }

    if (mb1Data->flags & (1<<9)) {
        MbLocalSetBootloader(mb1Data->bootLoaderName);
    }

    if (mb1Data->flags & (1<<10)) {
        struct Mb1ApmTable *apm = mb1Data->apmTable;
        MbLocalSetApm(apm->apmVersion, apm->apmCSeg, apm->apmOffset, apm->apmCSeg16, apm->apmDSeg, apm->apmFlags, 
                apm->apmCSegLen, apm->apmCSeg16Len, apm->apmDSegLen);
    }

    if (mb1Data->flags & (1<<11)) {
        MbLocalSetVbe(mb1Data->vbeMode, mb1Data->vbeInterfaceSeg, mb1Data->vbeInterfaceOff, mb1Data->vbeInterfaceLen, 
                (uint8_t *)mb1Data->vbeControlInfo, (uint8_t *)mb1Data->vbeModeInfo);

        // -- We can only assume that GRUB is willing to provide these fields
        char *l = mb1Data->bootLoaderName;
        if (l[0] == 'G' && l[1] == 'R' && l[2] == 'U' && l[3] == 'B') {
            MbLocalSetFb(mb1Data->fbAddr, mb1Data->fbPitch, mb1Data->fbWidth, mb1Data->fbHeight, mb1Data->fbBpp, 
                    mb1Data->fbType);

            if (mb1Data->fbType == 0) {
                for (uint32_t i = 0; i < mb1Data->pallet.fbPalletNumColors; i ++) {
                    struct MultibootColor *color = &(((struct MultibootColor *)mb1Data->pallet.fbPalletAddr)[i]);
                    MbLocalAddPalletColor(color->red, color->green, color->blue);
                }
            }
        }
    }
}
