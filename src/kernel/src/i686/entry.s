#####################################################################################################################
##
##  kernel i686/entry.s -- This is the entry point for the x86 32-bit processors
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
## Note that when testing with QEMU using the -kernel option, the kernel files MUST be in a multi-boot compliant 
## format.  Therefore, the multiboot stuff will be included here even if it not yet used.
##
## -----------------------------------------------------------------------------------------------------------------
##
##     Date     Tracker  Version  Pgmr  Description
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-09  Initial   0.0.0   ADCL  Initial version
##
#####################################################################################################################


.globl          _start
.globl          multiboot_header


#
# -- Include Multiboot information for testing with QEMU
#    ---------------------------------------------------
.section        .mboot


#define magic 0x1badb002
#define flags 0

multiboot_header:
    .long       magic
    .long       flags
    .long       (-magic-flags)


#
# -- This is the section that gets control on execution
#    --------------------------------------------------
.section        .init


#
# -- This is the main entry point for the OS.  Is a multi-core environment, only the Boot Processor get control 
#    here; the others are halted.  
#    ---------------------------------------------------------------------------------------------------------------
_start:
# -- Set up the stack pointer.
    movl    %esp,(stack+0x1000)

    jmp     kMain


.comm stack,0x1000
