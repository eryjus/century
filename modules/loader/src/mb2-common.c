//===================================================================================================================
//
//  loader mb2-common.c -- Multiboot 2 functions that are common to multiple architectures
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


//#define DEBUG_MB2

#include "types.h"
#include "proto.h"
#include "mb2.h"
#include "mb-local.h"


//
// -- This is the multiboot 1 information structure (will remain NULL if not booted with MB1-compliant loader)
//    --------------------------------------------------------------------------------------------------------
struct Mb2Fixed *mb2Data = 0;


//-------------------------------------------------------------------------------------------------------------------
// ReadMB2Info() -- Read the multiboot 2 information from the data provided and store it locally
//-------------------------------------------------------------------------------------------------------------------
void ReadMB2Info(void)
{
    if (!mb2Data) return;

#ifdef DEBUG_MB2
    kprintf(u8"Reading Multiboot2 Information (we have a structure at %p)\n", mb2Data);
#endif

    uint32_t locn = (uint32_t)mb2Data + sizeof(struct Mb2Fixed);
    bool lastTag = false;

#ifdef DEBUG_MB2
    kprintf(u8"The starting location of the MB2 Info Block is %p\n", locn);
#endif

    while (!lastTag) {
        struct Mb2BasicTag *tag = (struct Mb2BasicTag *)locn;
        switch (tag->type) {
        case MB2_TAG_LAST_TAG:
            lastTag = true;
            break;

        case MB2_TAG_CMD_LINE:
#ifdef DEBUG_MB2
            kprintf(u8"MB2 Command Line\n", locn);
#endif

            MbLocalSetCmdLine(((struct Mb2CmdLine *)locn)->cmdLine);
            break;

        case MB2_TAG_LOADER:
#ifdef DEBUG_MB2
            kprintf(u8"MB2 Loader Name\n", locn);
#endif

            MbLocalSetBootloader(((struct Mb2Loader *)locn)->name);
            break;

        case MB2_TAG_MODULE: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 Module\n", locn);
#endif

            struct Mb2Module *mod = (struct Mb2Module *)locn;
            kprintf("Adding module %s\n", mod->name);
            MbLocalAddModule(mod->modStart, mod->modEnd, mod->name);
            break;
        }

        case MB2_TAG_BASIC_MEM: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 Basic Memory\n", locn);
#endif

            struct Mb2BasicMem *mem = (struct Mb2BasicMem *)locn;
            MbLocalBasicMem(mem->memLower, mem->memUpper);
            break;
        }

        case MB2_TAG_BOOT_DEV: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 Boot Device\n", locn);
#endif

            struct Mb2BootDevice *dev = (struct Mb2BootDevice *)locn;
            MbLocalSetBootDev(dev->biosDev, dev->partition, dev->subPartition, 0xffffffff);
            break;
        }

        case MB2_TAG_MMAP: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 Memory Map\n", locn);
#endif

            struct Mb2MemMap *mmap = (struct Mb2MemMap *)locn;
            uint32_t s = tag->size / mmap->entrySize;
            for (uint32_t i = 0; i < s; i ++) {
                MbLocalAddMmapEntry(mmap->entries[i].baseAddr, mmap->entries[i].length, mmap->entries[0].type);
                uint64_t newLimit = mmap->entries[i].baseAddr + mmap->entries[i].length;
                if (newLimit > GetMemAmount()) SetMemAmount(newLimit);
            }

            kprintf(u8"Found 0x%08lx %08lx bytes of memory\n", (uint32_t)(GetMemAmount() >> 32), (uint32_t)(GetMemAmount() & 0xffffffff));
            uint32_t memSize = (uint32_t)(GetMemAmount() >> 12);
            kprintf(u8"  0x%lx pages; bitmap frames 0x%lx\n", memSize, (memSize >> (12 + 3)) + (memSize&0x7fff?1:0));

            break;
        }

        case MB2_TAG_VBE: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 VBE Info\n", locn);
#endif

            struct Mb2VbeInfo *vbe = (struct Mb2VbeInfo *)locn;
            MbLocalSetVbe(vbe->vbeMode, vbe->vbeInterfaceSeg, vbe->vbeInterfaceOff, vbe->vbeInterfaceLen, 
                    vbe->vbeControlInfo, vbe->vbeModeInfo);

            break;
        }

        case MB2_TAG_FRAMEBUFFER: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 Framebuffer Info\n", locn);
#endif

            struct Mb2FbInfo *fb = (struct Mb2FbInfo *)locn;
            MbLocalSetFb(fb->fbAddr, fb->fbPitch, fb->fbWidth, fb->fbHeight, fb->fbBpp, fb->fbType);

            if (fb->fbType == 0) {
                for (uint32_t i = 0; i < fb->pallet.palletColors; i ++) {
                    MbLocalAddPalletColor(fb->pallet.color[i].red, fb->pallet.color[i].green, 
                            fb->pallet.color[i].blue);
                }
            }

            break;
        }

        case MB2_TAG_ELF_SYMS: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 ELF Symbols\n", locn);
#endif

            struct Mb2ElfSymbols *elf = (struct Mb2ElfSymbols *)locn;
            MbLocalSetElfSyms(elf->num, elf->entSize, elf->shndx);

            break;
        }

        case MB2_TAG_APM: {
#ifdef DEBUG_MB2
            kprintf(u8"MB2 APM Table\n", locn);
#endif

            struct Mb2Apm *apm = (struct Mb2Apm *)locn;
            MbLocalSetApm(apm->version, apm->cseg, apm->offset, apm->cseg16, apm->dseg, apm->flags, apm->csegLen, 
                    apm->cseg16Len, apm->dsegLen);

            break;
        }

        default:
            kprintf(u8"Unimplemented MB2 type %d\n", tag->type);
            break;
        }

        locn += (tag->size + (~(tag->size - 1) & 0x7));
#ifdef DEBUG_MB2
        kprintf(u8"Incrementing the location by 0x%x to %p\n", tag->size, locn);
#endif
    }
}