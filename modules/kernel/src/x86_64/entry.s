#####################################################################################################################
##
##  kernel x86_64/entry.s -- This is the entry point for the x86_64 64-bit processors
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
## TODO:
## Now, with that said, I am unable at this point to get x86_64-elf-gcc to compile code into an elf-64 object
## file and insert 32-bit code.   I know how to do this with nasm, but that is not in my current tool-chain
## since I am cross-assembling into arm as well.  I really do not want to have to write for different assemplers
## based on the hardware architecture.  On top of that, I am not sure how to get gcc to comply with my bit-size
## wishes.  For now, this will simply hlt and loop; it will not output anything to the serial port.
##
## -----------------------------------------------------------------------------------------------------------------
##
##     Date     Tracker  Version  Pgmr  Description
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-09  Initial   0.0.0   ADCL  Initial version
##
#####################################################################################################################


.arch           i686
.globl          _start
.globl          multiboot_header


#
# -- Include Multiboot information for testing with QEMU
#    ---------------------------------------------------
.section        .mboot


#define MAGIC   0x1badb002
#define FLAGS   0

#define MAGIC2  0xe85250d6
#define LEN     MultibootHeader2End - MultibootHeader2
#define CHECK2  (-(MAGIC2 + 0 + LEN) & 0xffffffff)


multiboot_header:
    .long       MAGIC
    .long       FLAGS
    .long       (-MAGIC-FLAGS)



.align          8
MultibootHeader2:      
    .long       MAGIC2 
    .long       0                               # architecture: 0=32-bit protected mode
    .long       LEN                             # total length of the mb2 header
    .long       CHECK2                          # mb2 checksum

.align          8 
Type4Start:        
    .word       4                               # type=4
    .word       1                               # not optional
    .long       Type4End-Type4Start             # size = 12
    .long       1<<1                            # EGA text support
Type4End:

.align          8
Type6Start:      
    .word       6                               # Type=6
    .word       1                               # Not optional
    .long       Type6End-Type6Start             # size = 8 bytes even tho the doc says 12
Type6End:

.align          8 
    .word       0                               # Type=0
    .word       0                               # flags=0
    .long       8                               # size=8
MultibootHeader2End:


#
# -- This is the section that gets control on execution
#    --------------------------------------------------
.section        .init


#
# -- This is the main entry point for the OS.  Is a multi-core environment, only the Boot Processor get control 
#    here; the others are halted.  
#    ---------------------------------------------------------------------------------------------------------------
_start:
    hlt
    jmp         _start