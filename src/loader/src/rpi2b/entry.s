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
@@ -----------------------------------------------------------------------------------------------------------------
@@
@@     Date     Tracker  Version  Pgmr  Description
@@  ----------  -------  -------  ----  ---------------------------------------------------------------------------
@@  2017-03-26  Initial   0.0.0   ADCL  Initial version
@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


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

call$:
loop$:
    wfi
    b       loop$                       @ go back and loop through more
