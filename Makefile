#####################################################################################################################
##                                                                                                                 
##  Makefile -- This is the core makefile for creating the century OS for any of the supported architectures            
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  The basic folder layout here is going to be as follows:
##
##  +- century
##    +- bin            // intermediate targets for executable files
##    | +- rpi2b        // this is code that is specific to the Raspberry Pi 2B
##    | +- x86          // this is code specific to the intel x86 32-bit processor
##	  +- inc            // include files common to all architectures -- for inclusion into modules
##    +- lib            // intermediate targets for library files
##    | +- rpi2b        // this is code that is specific to the Raspberry Pi 2B
##    | +- x86          // this is code specific to the intel x86 32-bit processor
##    +- src            // source files common to all architectures
##    | +- kernel       // this is the kernel code
##    | | +- inc        // include files internal to the kernel
##    | | +- src        // source files internal to the kernel  
##    | |   +- rpi2b    // this is low-level code that is specific to the Raspberry Pi 2B
##    | |   +- x86      // this is low-level code specific to the intel x86 32-bit processor
##    | +- libc         // this is the kernel interface runtime libraries
##    | | +- inc        // include files internal to libc
##    | | +- src        // source files internal to libc  
##    | +- module1      // this is executable module #1
##    | | +- inc        // internal includes
##    | | +- src        // internal source  
##    | +- module2      // this is executable module #2
##    |   +- inc        // internal includes
##    |   +- src        // internal source
##    +- obj			// intermediate targets for object files
##    | +- rpi2b        // this is code that is specific to the Raspberry Pi 2B
##    | +- x86          // this is code specific to the intel x86 32-bit processor
##    +- sysroot        // this is the root files system for each architecture
##      +- rpi2b        // this is code that is specific to the Raspberry Pi 2B
##      +- x86          // this is code specific to the intel x86 32-bit processor
##                                                                                                                 
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-03-26  Initial   0.0.0   ADCL  Initial version 
## 
#####################################################################################################################


.SILENT:              # we don't need to see all the verbose stuff

#
# -- There are several kinds of targets we can make, and we want to make sure we can extract the architecture 
#    from the target; if the target is all, then we will handle this specially
#    --------------------------------------------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "all")
ARCH			:= 
else
ARCH            != ./tools/get-arch.sh $(MAKECMDGOALS)
endif


#
# -- Here we need to determine all the Makefrag files; these will be:
#    1) kernel (architecture specific)
#    2) libc/libk
#    3) generic
#    ----------------------------------------------------------------
MAKE-FRAG       := $(wildcard src/kernel/src/**/Makefrag) $(wildcard src/**/Makefrag)
MAKE-FRAG		:= $(sort $(MAKE-FRAG))


#
# -- Here we need to determine all the supported architectures -- determined by the Arch folders in kernel
#    -----------------------------------------------------------------------------------------------------
ARCH-LIST       := $(dir $(wildcard src/kernel/src/**/Makefrag))
ARCH-LIST       := $(subst src/kernel/src/,,$(ARCH-LIST))
ARCH-LIST       := $(subst /,,$(ARCH-LIST))


#
# -- Here we get a complete list of all the makefiles upon which the software builds require
#    ---------------------------------------------------------------------------------------
MAKE-FILES		:= Makefile $(MAKE-FRAG)


#
# -- We need to define several empty lists that will be populated by the Makefrags
#    -----------------------------------------------------------------------------
ALL             :=
OS-INCL         :=
CLEAN           :=
DEPEND          :=

ASM-PARM		:= -Wa,


#
# -- Assume we want to make a comprehensive list of all architectures
#    ----------------------------------------------------------------
.PHONY: all
all: all-tag


#
# -- Dump some key variables
#    -----------------------
.PHONY: disp-vars
disp-vars:
	echo MAKECMDGOALS=$(MAKECMDGOALS)
	echo
	echo ALL=$(ALL)
	echo ARCH=$(ARCH)
	echo ARCH-LIST=$(ARCH-LIST)
	echo MAKE-FILES=$(MAKE-FILES)
	echo MAKE-FRAG=$(MAKE-FRAG)
	echo OS-INCL=$(OS-INCL)


#
# -- At this point, include all the individual Makefrag files that are specific to each module and/or architecture
#    -------------------------------------------------------------------------------------------------------------
include $(MAKE-FRAG)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPEND)
endif

#
# -- An interim target to make sure we get all the Makefrags loaded
#    --------------------------------------------------------------
.PHONY: all-tag
all-tag: $(ALL) 


#
# -- Clean up the build
#    ------------------
.PHONY: clean
clean: $(CLEAN)
