##===================================================================================================================
##
##  loader x86_64/entry.s -- This is the entry point for the x86 64-bit processors (in 32-bit protected mode)
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp# Revision 42):
##        <hobbyos#eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  Both multiboot 1 and 2 standards are supported.  This means that this loader can be booted by either GRUB
##  and GRUB2.  We will let GRUB establish the video resolution.
##
## -----------------------------------------------------------------------------------------------------------------
##
##     Date     Tracker  Version  Pgmr  Description
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-14  Initial   0.0.0   ADCL  Initial version
##
##===================================================================================================================


#
# -- Expose some global addresses
#    ----------------------------
.globl          _start
.globl          systemFont
.globl          Halt


#
# -- This is the multiboot header.  During the link process, it will be relocated to the beginning of the 
#    resulting ELF file.  This is required since the multiboot header needs to be in the first 4/8K (I think).
#    ---------------------------------------------------------------------------------------------------------
.section        .mboot


#
# -- The following are used to populate the multiboot v1 header
#    ----------------------------------------------------------
#define MAGIC   0x1badb002
#define FLAGS   1<<2                    # set video

#
# -- The following are used to populate the multiboot v2 header
#    ----------------------------------------------------------
#define MAGIC2  0xe85250d6
#define LEN     MultibootHeader2End - MultibootHeader2
#define CHECK2  (-(MAGIC2 + 0 + LEN) & 0xffffffff)


#
# -- Standard Video resolution
#    -------------------------
#define MODE_TYPE   0
#define WIDTH   1280
#define HEIGHT  1024
#define DEPTH   16


#
# -- This is the multiboot 1 header
#    ------------------------------
multiboot_header:
# -- magic fields
    .long       MAGIC
    .long       FLAGS
    .long       -MAGIC-FLAGS
# -- address fields (unused placeholders)
    .long       0
    .long       0
    .long       0
    .long       0
    .long       0
# -- video fields
    .long       MODE_TYPE
    .long       WIDTH
    .long       HEIGHT
    .long       DEPTH


#
# -- This is the multiboot 2 header
#    ------------------------------
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
Type5Start:
    .word       5                               # graphic mode 
    .word       1                               # not optional
    .long       Type5End-Type5Start             # size = 20
    .long       WIDTH                           # 1280
    .long       HEIGHT                          # 1024
    .long       DEPTH                           # 16                              
Type5End:

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
# -- This section holds the main entry point.  Again, it will be located behind the multiboot header.
#    ------------------------------------------------------------------------------------------------
.section        .init

_start:
    cmp         %eax,0x2badb002
    jne         Halt
    mov         mb1Data,%ebx
    jmp         LoaderMain

Halt:
    cli
    hlt
    jmp         Halt


#
# -- This is where we include the binary data for the system font
#    ------------------------------------------------------------
.section        .data
systemFont:
.incbin         "system-font.bin"

