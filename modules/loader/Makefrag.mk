#####################################################################################################################
##                                                                                                                 
##  loader Makefrag.mk -- This file controls making the loader for any of its architectures (next after multioot)
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  Within this Makefrag.mk and its subordinate ones, we need to make sure that the following make goals populate 
##  the $(CURRENT-TARGET) macro:
##  make
##  make all
##  make <arch>
##  make loader
##  make <arch>-loader
##  make <arch>-iso
##  make run-<arch>
##
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-13  Initial   0.0.0   ADCL  Initial version
## 
#####################################################################################################################


#
# -- The subordinate make files
#    --------------------------
LOADER-MAKE-FRAG       			:= $(wildcard modules/loader/src/**/Makefrag.mk)
LOADER-MAKE-FRAG				:= $(sort $(LOADER-MAKE-FRAG))


#
# -- Some things common to all loader
#    ---------------------------------
LOADER-SRC						:= modules/loader/src
LOADER-INC						:= modules/loader/inc
LOADER-OBJ						:= obj/loader
LOADER-BIN						:= bin
LOADER-SYSROOT					:= sysroot


#
# -- Include the individual make files for each loader architecture
#    --------------------------------------------------------------
include $(LOADER-MAKE-FRAG)


#
# -- Make the loader for all architectures
#    -------------------------------------
loader: current-target
