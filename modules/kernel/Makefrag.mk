#####################################################################################################################
##                                                                                                                 
##  kernel Makefrag.mk -- This file controls making the kernel for any of its architectures
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
## Within this Makefrag.mk and its subordinate ones, we need to make sure that the following make goals populate 
## the $(CURRENT-TARGET) macro:
## make
## make all
## make <arch>
## make kernel
## make <arch>-kernel
## make <arch>-iso
## make run-<arch>
##
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-11  Initial   0.0.0   ADCL  Initial version 
## 
#####################################################################################################################


#
# -- The subordinate make files
#    --------------------------
KERNEL-MAKE-FRAG       			:= $(wildcard modules/kernel/src/**/Makefrag.mk)
KERNEL-MAKE-FRAG				:= $(sort $(KERNEL-MAKE-FRAG))


#
# -- Some things common to all kernels
#    ---------------------------------
KERNEL-SRC						:= modules/kernel/src
KERNEL-INC						:= modules/kernel/inc
KERNEL-OBJ						:= obj/kernel
KERNEL-BIN						:= bin
KERNEL-SYSROOT					:= sysroot


#
# -- Include the individual make files for each kernel architecture
#    --------------------------------------------------------------
include $(KERNEL-MAKE-FRAG)


#
# -- Make the kernel for all architectures
#    -------------------------------------
kernel: current-target
