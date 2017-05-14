@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@
@@  loader rpi2b/entry.s -- This is the entry point for the Raspberry Pi 2B
@@
@@        Copyright (c)  2017 -- Adam Clark
@@
@@        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
@@        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
@@        you can do whatever you want with this stuff. If we meet some day, and you
@@        think this stuff is worth it, you can buy me a beer in return.
@@
@@  A couple of notes here... 1) multiboot2 is not currently supported by rpi-boot ao I am leaving these structures
@@  out of this file; 2) video setup is not supported by rpi-boot so I am leaving the definitions out of these 
@@  structures.
@@
@@ -----------------------------------------------------------------------------------------------------------------
@@
@@     Date     Tracker  Version  Pgmr  Description
@@  ----------  -------  -------  ----  ---------------------------------------------------------------------------
@@  2017-03-26  Initial   0.0.0   ADCL  Initial version
@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


@
@ -- This is the multiboot header.  During the link process, it will be relocated to the beginning of the 
@    resulting ELF file.  This is required since the multiboot header needs to be in the first 4/8K (I think).
.section        .mboot

.globl          systemFont
.globl          Halt


@
@ -- The following are used to populate the multiboot v1 header
@    ----------------------------------------------------------
#define MAGIC   0x1badb002
#define FLAGS   1<<1 | 1<<2

@
@ -- This is the multiboot 1 header
@    ------------------------------
multiboot_header:
@ -- magic fields
    .long       MAGIC
    .long       FLAGS
    .long       -MAGIC-FLAGS


.section        .init
.globl          _start


@
@ -- This is the main entry point for the OS.  Control is handed to all 4 cores to this point.  This means that
@    all 4 cores will be executing hte same code at the same time, battling over the which core gets to do what.
@    Ultimately, we need to turn off all the cores except core @0 and let that core take care of the initialization.
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
@ -- This is the code for CPU @0 to execute
@    --------------------------------------
.section    .text

initialize:
    ldr     sp,=0x800                    @ set up a stack


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

call$:
    ldr     r3,=0x2badb002
    cmp     r0,r3                       @ check for MB1 signature
    ldreq   r3,=mb1Data                 @ save the MB1 address
    streq   r1,[r3]
    bleq    LoaderMain

    ldr     r3,=0x36d76289 
    cmp     r0,r3                       @ check for MB2 signature
    ldreq   r3,=mb2Data                 @ save the MB@ address
    streq   r1,[r3]
    bl      LoaderMain

Halt:
    wfi
    b       Halt                        @ go back and loop through more


#
# -- This is where we include the binary data for the system font
#    ------------------------------------------------------------
.section        .rodata
systemFont:
.incbin         "system-font.bin"


.section        .bootstack
stack32:        
    .rept       1024
    .byte       0
    .endr
stackend:
    