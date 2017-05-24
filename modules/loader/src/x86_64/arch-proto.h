//===================================================================================================================
//
//  loader x86_64/proto.h -- Architecture specific prototypes
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
//  2017-04-19  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#ifndef __ARCH_PROTO_H_INCLUDED__
#define __ARCH_PROTO_H_INCLUDED__


//
// -- Some MMU address locations
//    --------------------------
#define MMU_USER_SPACE  0x0000000000000000
#define MMU_KERNEL      0xffff800000000000
#define MMU_SLAB        0xffffa00000000000
#define MMU_UNUSED      0xffffc00000000000
#define MMU_FRAME_BUF   0xffffe00000000000
#define MMU_POISON      0xfffff00000000000
#define MMU_STACKS      0xffffff0000000000
#define MMU_TEMP        0xffffff4000000000
#define MMU_PT          0xffffff8000000000
#define MMU_PD          0xffffffffc0000000
#define MMU_PDPT        0xffffffffffe00000
#define MMU_PML4        0xfffffffffffff000


//
// -- architecture-specific type sizes
//    --------------------------------
#define BITNESS 64
typedef uint64_t arch_addr_t;                    // This is the size of an address in this architecture
typedef uint32_t addr_t;                        // This is the size of an address in the current cpu bit model


//
// -- This is a dummy function to return 0 for the system timer; calling function can handle
//    --------------------------------------------------------------------------------------
inline static uint64_t ReadSysTimerCount(void) { return 0; }


//
// -- Read/Write to hardware port
//    ---------------------------
uint8_t inb(uint16_t port);
uint8_t outb(uint16_t port, uint8_t byte);


//
// -- Read BDA Data
//    -------------
void ReadBda(void);


#endif
