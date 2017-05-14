##===================================================================================================================
##
##  loader i686/entry.s -- This is the entry point for the x86 32-bit processors
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
##  2017-04-09  Initial   0.0.0   ADCL  Initial version
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
#define FLAGS   1<<1 | 1<<2                    # get mem & set video

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
#define WIDTH   1024
#define HEIGHT  768
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
    .long       0                               # graphics support
Type4End:

.align          8
Type5Start:
    .word       5                               # graphic mode 
    .word       1                               # not optional
    .long       Type5End-Type5Start             # size = 20
    .long       WIDTH                           # 640
    .long       HEIGHT                          # 480
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

GdtDesc:
    .word       24-1                            # size minus one...
    .long       gdt                             # this is the addresss


#
# -- This section holds the main entry point.  Again, it will be located behind the multiboot header.
#    ------------------------------------------------------------------------------------------------
.section        .init

_start:
    mov         $GdtDesc,%ecx                   # Get the gdt address
    lgdt        (%ecx)                          # and load the GDT

    mov         $0x10,%ecx
    mov         %cx,%ds
    mov         %cx,%es
    mov         %cx,%fs
    mov         %cx,%gs
    mov         %cx,%ss                         # halts interrupts for one more instruction to set up stack
    mov         $0x200000,%esp                  # set up a stack
    
    push        $0x08
    push        $newGDT
    retf                                        # an almost jump, returning to an address rather than jump    

newGDT:
    cmp         $0x2badb002,%eax
    jne         chkMB2
    mov         %ebx,mb1Data
    jmp         ldrMain

chkMB2:
    cmp         $0x36d76289,%eax
    jne         ldrMain
    mov         %ebx,mb2Data

ldrMain:
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

