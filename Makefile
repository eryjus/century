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
## There is a serious need to some strict standards with all the different setups we will need to be able to 
## support.  All of the explicit targets and string macros will have the following format:
##
##        ARCH-MODULE-NAME
##
## Where:
## * ARCH is the architecture of the target to build
## * MODULE is the module that will be build 
## * NAME is the actual thing that is being referred to
##
## Additionally, the following common targets will be supported in all aspects
## make 				-- make everything (alias for `make all`)
## make all 			-- make everything, including the iso images
## make clean 			-- clean up everything
## make <arch>			-- make everything for the architecture, updating up to sysroot (not the iso)
## make <module>		-- make the module for all architectures, updating up to sysroot (not the iso)
## make <arch>-<module>	-- make the module/architecture combination, updating up to sysroot (not the iso)
## make <arch>-iso 		-- make an iso image for the architecture
## make run-<arch>		-- make an indo the the architecture and run it with QEMU
##
## For the 'all' and 'clean' targets, clobal variables have been created here that the individual Makefrag.mk 
## files are required to update to support.  Note the position of these in this file -- the order the files are 
## read matters.  Therefore, the $(ALL) variable is only read after all the other Makefrag.mk files are read.
## clean is similar.
##
## The problem will be the _arch_-iso, which will be dependent on several other Makefrag.mk files.  To get around 
## this, we will construct a string macro called $(CURRENT-TARGET) that will be built based on the actual target
## requested.  $(CURRENT-TARGET) is required to be constructed in the individual Makefrag.mk files.
##                                                                                                                 
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-03-26  Initial   0.0.0   ADCL  Initial version 
## 
#####################################################################################################################


#.SILENT:              # we don't need to see all the verbose stuff

#
# -- There are several kinds of targets we can make, and we want to make sure we can extract the architecture 
#    from the target; if the target is all, then we will handle this specially
#    --------------------------------------------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "all")
ARCH							:= 
else
ARCH            				!= ./tools/get-arch.sh $(MAKECMDGOALS)
endif


#
# -- Here we need to determine all the Makefrag files; these will be:
#    1) kernel (architecture specific)
#    2) libc/libk
#    3) generic
#    ----------------------------------------------------------------
MAKE-FRAG       				:= $(wildcard src/**/Makefrag.mk)
MAKE-FRAG						:= $(sort $(MAKE-FRAG))


#
# -- Here we need to determine all the supported architectures -- determined by the Arch folders in kernel
#    -----------------------------------------------------------------------------------------------------
ARCH-LIST       				:= $(dir $(wildcard src/kernel/src/**/Makefrag.mk))
ARCH-LIST       				:= $(subst src/kernel/src/,,$(ARCH-LIST))
ARCH-LIST       				:= $(subst /,,$(ARCH-LIST))


#
# -- Here we get a complete list of all the makefiles upon which the software builds require
#    ---------------------------------------------------------------------------------------
MAKE-FILES						:= Makefile $(MAKE-FRAG)


#
# -- We need to define several empty lists that will be populated by the Makefrags
#    -----------------------------------------------------------------------------
ALL             				:=
OS-INCL         				:=
CLEAN           				:=
DEPEND          				:=
CURRENT-TARGET  				:=
ISO								:=


#
# -- Some things need to be defined at the highest level, here
#    ---------------------------------------------------------
ASM-PARM						:= -Wa,


#
# -- Assume we want to make a comprehensive list of all architectures
#    ----------------------------------------------------------------
.PHONY: all
all: current-target


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
# -- This is an important rule!!!!  This is the key to the flexible build targets in the build system.  Nearly 
#    all possible targets will be dependent on this target, especially when there are dependencied across 
#    more than one module.  Each module will then be responsible for populating the value of $(CURRENT_TARGET).
#    ----------------------------------------------------------------------------------------------------------
.PHONY: current-target
current-target: $(CURRENT-TARGET) $(ISO)


#
# -- Clean up the build
#    ------------------
.PHONY: clean
clean: $(CLEAN)
	rm -fR bin
	rm -fR obj
	rm -fR iso
	rm -fR sysroot
