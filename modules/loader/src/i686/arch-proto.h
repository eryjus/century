//===================================================================================================================
//
//  loader i686/proto.h -- Architecture specific function prototypes
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
#define MMU_USER_SPACE  0x00000000
#define MMU_KERNEL      0x80000000
#define MMU_SLAB        0xa0000000
#define MMU_UNUSED      0xc0000000
#define MMU_FRAME_BUF   0xe0000000
#define MMU_POISON      0xf0000000
#define MMU_STACKS      0xff000000
#define MMU_TEMP        0xff800000
#define MMU_RECURSIVE   0xffc00000


//
// -- architecture-specific type sizes
//    --------------------------------
#define BITNESS 32
typedef uint32_t arch_addr_t;                   // This is the size of an address in this architecture
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
