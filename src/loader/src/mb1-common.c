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

#include "mb1.h"
#include "proto.h"


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
    kprintf("Reading Multiboot1 Information (we have a structure at %p)\n", mb1Data);
    kprintf("  Flags: %p\n", mb1Data->flags);

    if (mb1Data->flags & (1<<0)) {
        kprintf("  The Memory data is valid:\n");
        kprintf("    Lower Memory %lu\n", mb1Data->memLower);
        kprintf("    Upper Memory %lu\n", mb1Data->memUpper);
    }

    if (mb1Data->flags & (1<<1)) {
        kprintf("  The Boot Device data is valid:\n");
        kprintf("    Disk %x\n", mb1Data->bootDevice & 0xff);
        kprintf("    Part 1 %x\n", (mb1Data->memUpper >> 8) & 0xff);
        kprintf("    Part 2 %x\n", (mb1Data->memUpper >> 16) & 0xff);
        kprintf("    Part 3 %x\n", (mb1Data->memUpper >> 24) & 0xff);
    }

    if (mb1Data->flags & (1<<2)) {
        kprintf("  The Command Line is valid:\n");
        if (! mb1Data->cmdLine) kprintf("    ... but is NULL\n");
        else if (strlen(mb1Data->cmdLine) == 0) kprintf("    ... but is empty\n");
        else kprintf("    %s\n", mb1Data->cmdLine);
    }

    if (mb1Data->flags & (1<<3)) {
        kprintf("  The Module Structures are valid:\n");
        kprintf("    There are %lu modules loaded\n", mb1Data->moduleCount);

        uint32_t i;
        struct Mb1Mods *m;

        for (m = mb1Data->modules, i = 0; i < mb1Data->moduleCount; i ++) {
            kprintf("    Module #%u\n", i);
            kprintf("      Loaded at %p, ending at %p\n", m[i].modStart, m[i].modEnd);
            kprintf("      Module Identifier: %s\n", m[i].modIdent);
        }
    }

    if (mb1Data->flags & (1<<4)) {
        kprintf("  The Symbol Table is valid:\n");
    }

    if (mb1Data->flags & (1<<5)) {
        kprintf("  The ELF Section Header is valid:\n");
    }

    if (mb1Data->flags & (1<<6)) {
        uint32_t size = mb1Data->mmapLength;
        kprintf("  The Memory Map Structures are valid (length = %x):\n", mb1Data->mmapLength);
        struct Mb1MmapEntry *entry = mb1Data->mmap;
        while (size) {
            kprintf("    At %x%08x: length %x%08x; type 0x%02x\n", (uint32_t)(entry->mmapAddr >> 32), 
                    (uint32_t)(entry->mmapAddr & 0xffffffff), (uint32_t)(entry->mmapLength >> 32), 
                    (uint32_t)(entry->mmapLength & 0xffffffff), entry->mmapType);

            size -= (entry->mmapSize + 4);
            entry = (struct Mb1MmapEntry *)(((uint32_t)entry) + entry->mmapSize + 4);
        }
    }

    if (mb1Data->flags & (1<<7)) {
        kprintf("  The Drivers Structures are valid:\n");
    }

    if (mb1Data->flags & (1<<8)) {
        kprintf("  The Config Table is valid:\n");
        kprintf("    The ROM Config Table Address is at %p\n", mb1Data->configTable);
    }

    if (mb1Data->flags & (1<<9)) {
        kprintf("  The Boot Loader Name is valid:\n");
        if (! mb1Data->bootLoaderName) kprintf("    ... but is NULL\n");
        else if (strlen(mb1Data->bootLoaderName) == 0) kprintf("    ... but is empty\n");
        else kprintf("    %s\n", mb1Data->bootLoaderName);
    }

    if (mb1Data->flags & (1<<10)) {
        kprintf("  The APM Table Structure is valid:\n");
    }

    if (mb1Data->flags & (1<<11)) {
        kprintf("  The VBE Data is valid:\n");
        kprintf("    The VBE Control Info structure is at %p\n", mb1Data->vbeControlInfo);
        kprintf("    The VBE Mode Info structure is at %p\n", mb1Data->vbeModeInfo);
        kprintf("    The Current VBE Mode is %d\n", mb1Data->vbeMode);
        kprintf("    The VBE Protected Mode Interface Table is at Seg:Off %x:%x\n", 
                mb1Data->vbeInterfaceSeg, mb1Data->vbeInterfaceOff);
        kprintf("    The VBE Protected Mode Interface Length is %d\n", mb1Data->vbeInterfaceLen);
        kprintf("    As a stretch goal, the frame buffer addr is at %llx\n", mb1Data->fbAddr);
    }

#endif

    //
    // -- Read the VBE information if available
    //    -------------------------------------
    if (!(mb1Data->flags & (1 << 11))) {
        UartPutS("No Frame Buffer Information");
        Halt();
    }
}
