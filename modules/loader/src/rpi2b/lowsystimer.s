@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@
@@  kernel rpi2b/lowsystimer.s -- This file contains low-level asm for interacting with the system timer.
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
@@  2017-04-04  Initial   0.0.0   ADCL  Initial version
@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include        "hw.h"

.section    .text
.globl		ReadSysTimerCount

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@ uint64_t ReadSysTimerCount() -- read the system time running count
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
ReadSysTimerCount:
	ldr		r0,=ST_CLO                  @ load the base address of the system timer
	ldrd	r0,r1,[r0]                  @ Get the 64-bit timer "count" into r1:r0
	mov		pc,lr						@ return

