;;@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@
@@  kernel rpi2b/entry.s -- This is the entry point for the Raspberry Pi 2B
@@
@@        Copyright (c)  2017 -- Adam Clark
@@
@@        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
@@        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
@@        you can do whatever you want with this stuff. If we meet some day, and you
@@        think this stuff is worth it, you can buy me a beer in return.
@@
@@ -----------------------------------------------------------------------------------------------------------------
@@
@@     Date     Tracker  Version  Pgmr  Description
@@  ----------  -------  -------  ----  ---------------------------------------------------------------------------
@@  2017-03-26  Initial   0.0.0   ADCL  Initial version
@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include        "hw.h"

@
@ -- This is the multiboot header.  During the link process, it will be relocated to the beginning of the 
@    resulting ELF file.  This is required since the multiboot header needs to be in the first 4/8K (I think).
.section        .mboot


@
@ -- The following are used to populate the multiboot v1 header
@    ----------------------------------------------------------
#define MAGIC   0x1badb002
#define FLAGS   1<<2                    @ set video

@
@ -- The following are used to populate the multiboot v2 header
@    ----------------------------------------------------------
#define MAGIC2  0xe85250d6
#define LEN     MultibootHeader2End - MultibootHeader2
#define CHECK2  (-(MAGIC2 + 0 + LEN) & 0xffffffff)


@
@ -- Standard Video resolution
@    -------------------------
#define MODE_TYPE   0
#define WIDTH   1280
#define HEIGHT  1024
#define DEPTH   16


@
@ -- This is the multiboot 1 header
@    ------------------------------
multiboot_header:
@ -- magic fields
    .long       MAGIC
    .long       FLAGS
    .long       -MAGIC-FLAGS
@ -- address fields (unused placeholders)
    .long       0
    .long       0
    .long       0
    .long       0
    .long       0
@ -- video fields
    .long       MODE_TYPE
    .long       WIDTH
    .long       HEIGHT
    .long       DEPTH


@
@ -- This is the multiboot 2 header
@    ------------------------------
.align          8
MultibootHeader2:      
    .long       MAGIC2 
    .long       0                               @ architecture: 0=32-bit protected mode
    .long       LEN                             @ total length of the mb2 header
    .long       CHECK2                          @ mb2 checksum

.align          8 
Type4Start:        
    .word       4                               @ type=4
    .word       1                               @ not optional
    .long       Type4End-Type4Start             @ size = 12
    .long       1<<1                            @ EGA text support
Type4End:

.align          8
Type5Start:
    .word       5                               @ graphic mode 
    .word       1                               @ not optional
    .long       Type5End-Type5Start             @ size = 20
    .long       WIDTH                           @ 1280
    .long       HEIGHT                          @ 1024
    .long       DEPTH                           @ 16                              
Type5End:

.align          8
Type6Start:      
    .word       6                               @ Type=6
    .word       1                               @ Not optional
    .long       Type6End-Type6Start             @ size = 8 bytes even tho the doc says 12
Type6End:

.align          8 
    .word       0                               @ Type=0
    .word       0                               @ flags=0
    .long       8                               @ size=8
MultibootHeader2End:


.section        .init
.globl          _start


@
@ -- This is the main entry point for the OS.  Control is handed to all 4 cores to this point.  This means that
@    all 4 cores will be executing hte same code at the same time, battling over the which core gets to do what.
@    Ultimately, we need to turn off all the cores except core #0 and let that core take care of the initialization.
@    In our case so far, this is the only core that will run.
@
@    This code is lifted from the ARM Cortex A-Series Version 4.0 programmer's Guide (Example 13-5).
@    ---------------------------------------------------------------------------------------------------------------
_start:
    mrc     p15,0,r3,c0,c0,5            @ Read Multiprocessor Affinity Register
    and     r3,r3,#0x3                  @ Extract CPU ID bits
    cmp     r3,#0
    beq     initialize                  @ if we’re on CPU0 goto the start

@ -- all other cores will drop in to this loop - a low power mode infinite loop
wait_loop:
    wfi                                 @ wait for interrupt
    b       wait_loop                   @ go back and do it again


@
@ -- This is the code for CPU #0 to execute
@    --------------------------------------
.section    .text

initialize:
    mov     sp,#0x8000                  @ set up a stack


@
@ -- Clear the .bss segment
@    ----------------------
    ldr     r4,=_bssStart               @ get the starting address of the bss section
    ldr     r9,=_bssEnd                 @ get the ending address of the bss section
    mov     r5,#0
    mov     r6,#0
    mov     r7,#0
    mov     r8,#0

bssClr$:
    cmp     r4,r9                       @ have we gotten to our address limit?
    bhs     call$

    stmia   r4!,{r5-r8}                 @ store 4 words at once
    b       bssClr$ 

@
@ -- call the kernel main function
@    -----------------------------
call$:
    ldr     r3,=kMain
    blx     r3

loop$:
    wfi
    b       loop$                       @ go back and loop through more
