##===================================================================================================================
##
##  loader i686/port-io.s -- Handle communication with the hardware ports
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
##  2017-04-22  Initial   0.0.0   ADCL  Initial version
##
##===================================================================================================================


.globl          inb
.globl          outb

#--------------------------------------------------------------------------------------------------------------------
# inb() -- get a byte from an I/O port
#--------------------------------------------------------------------------------------------------------------------
inb:
    push        %ebp
    mov         %esp,%ebp

    mov         8(%ebp),%edx
    xor         %eax,%eax
    in          %dx,%al

    pop         %ebp
    ret


#--------------------------------------------------------------------------------------------------------------------
# outb() -- send a byte to an I/O port
#--------------------------------------------------------------------------------------------------------------------
outb:
    push        %ebp
    mov         %esp,%ebp

    mov         8(%ebp),%edx
    mov         12(%ebp),%eax
    out         %al,%dx

    pop         %ebp
    ret
