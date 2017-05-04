#####################################################################################################################
##
##  kernel i686/ioport.s -- Functions to interface with the x86 I/O ports
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
## -----------------------------------------------------------------------------------------------------------------
##
##     Date     Tracker  Version  Pgmr  Description
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-09  Initial   0.0.0   ADCL  Initial version
##
#####################################################################################################################


.section        .text
.globl          ReadIoPort
.globl          WriteIoPort
.globl          ReadSysTimerCount


#####################################################################################################################
## uint8_t ReadIoPort(uint32_t) -- Read from an x86 I/O port
#####################################################################################################################
ReadIoPort:
    mov         4(%esp),%edx
    xor         %eax,%eax
    inb         %dx,%al
    ret


#####################################################################################################################
## void WriteIoPort(uint32_t, uint8_t) -- Write to an x86 I/O port
#####################################################################################################################
WriteIoPort:
    mov         4(%esp),%edx
    mov         8(%esp),%eax
    outb        %al,%dx
    ret


#####################################################################################################################
## uint64_t ReadSysTimerCount(void) -- Dummy function to pretend to read a system timer (caller to handle)
#####################################################################################################################
ReadSysTimerCount:
    xor         %edx,%edx
    xor         %eax,%eax
    ret
