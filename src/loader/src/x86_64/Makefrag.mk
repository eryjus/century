#####################################################################################################################
##                                                                                                                 
##  loader x86_64/Makefrag.mk -- This file controls making the loader for the x86_64 architecture (after multioot)
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  Within this Makefrag.mk, we need to make sure that the following make goals populate the $(CURRENT-TARGET) 
##  macro:
##  make
##  make all
##  make x86_64
##  make loader
##  make x86_64-loader
##  make x86_64-iso
##  make run-x86_64
##
##  This target is really a 32-bit application that prepares the system to jump into long mode.  The kernel will 
##  be in long mode as soon as we make the jump to the kernel entrty point.
##
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-14  Initial   0.0.0   ADCL  Initial version
## 
#####################################################################################################################


#
# -- File Location definitions
#    -------------------------
X86_64-LOADER-SRC				:= $(LOADER-SRC)/x86_64
X86_64-LOADER-OBJ				:= $(LOADER-OBJ)/x86_64
X86_64-LOADER-BIN				:= $(LOADER-BIN)/x86_64
X86_64-LOADER-SYSROOT			:= $(LOADER-SYSROOT)/x86_64


#
# -- File Definitions
#    ----------------
X86_64-LOADER-S   				:= $(notdir $(wildcard $(X86_64-LOADER-SRC)/*.s) $(wildcard $(LOADER-SRC)/*.s))
X86_64-LOADER-C   				:= $(notdir $(wildcard $(X86_64-LOADER-SRC)/*.c) $(wildcard $(LOADER-SRC)/*.c))
X86_64-LOADER-O   				:= $(sort $(subst .s,.o,$(X86_64-LOADER-S)) $(subst .c,.o,$(X86_64-LOADER-C)))
X86_64-LOADER-D   				:= $(subst .o,.d,$(X86_64-LOADER-O))
X86_64-LOADER-LS  				:= $(X86_64-LOADER-SRC)/loader.ld


#
# -- Target definitions
#    ------------------
X86_64-LOADER-IMG 				:= $(X86_64-LOADER-BIN)/loader.img
X86_64-LOADER-ELF 				:= $(X86_64-LOADER-BIN)/loader.elf
X86_64-LOADER-SYS 				:= $(subst $(X86_64-LOADER-BIN),$(X86_64-LOADER-SYSROOT)/boot,$(X86_64-LOADER-ELF))


#
# -- Some macros to help with finding the include files and building this component
#    ------------------------------------------------------------------------------
X86_64-LOADER-IA  				:= -Wa,-I -Wa,$(X86_64-LOADER-SRC) -Wa,-I -Wa,$(LOADER-INC)
X86_64-LOADER-IC  				:= $(subst $(ASM-PARM),,$(X86_64-LOADER-IA))
X86_64-LOADER-PREFIX			:= i686-elf


#
# -- Add to the global vars
#    ----------------------
ALL              				+= $(X86_64-LOADER-SYS)
CLEAN            				+= x86_64-loader-clean
DEPEND           				+= $(addprefix $(X86_64-LOADER-OBJ)/,$(X86_64-LOADER-D))


#
# -- Build Tool definitions
#    ----------------------
X86_64-LOADER-AS  				:= $(X86_64-LOADER-PREFIX)-gcc -fpic -ffreestanding -x assembler-with-cpp \
		$(X86_64-LOADER-IA) -Wall -Werror -c
X86_64-LOADER-CC  				:= $(X86_64-LOADER-PREFIX)-gcc -fpic -ffreestanding $(X86_64-LOADER-IC) -Wall \
		-Werror -c
X86_64-LOADER-DEP 				:= $(X86_64-LOADER-PREFIX)-cpp -M -ffreestanding $(X86_64-LOADER-IC)
X86_64-LOADER-LD  				:= $(X86_64-LOADER-PREFIX)-gcc -T $(X86_64-LOADER-LS) -ffreestanding -O2 -nostdlib 
X86_64-LOADER-LIBS  				:= 
X86_64-LOADER-OBJCOPY  			:= $(X86_64-LOADER-PREFIX)-objcopy


#
# -- Make sure that $(CURRENT-TARGET) is set up properly based on the goal
#    ---------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "")
CURRENT-TARGET					+= $(X86_64-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "all")
CURRENT-TARGET					+= $(X86_64-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "x86_64")
CURRENT-TARGET					+= $(X86_64-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "loader")
CURRENT-TARGET					+= $(X86_64-LOADER-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "x86_64-loader")
CURRENT-TARGET					+= $(X86_64-LOADER-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "x86_64-iso")
ISO								+= $(X86_64-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "run-x86_64")
ISO								+= $(X86_64-LOADER-SYS)
endif


#
# -- The $(X86_64-LOADER-SYS) target, which will cover 3 of the 7 targets above
#    --------------------------------------------------------------------------
$(X86_64-LOADER-SYS): $(X86_64-LOADER-ELF)
	echo "X86_64-SYSROOT:" $(notdir $@)	
	mkdir -p $(dir $@)
	rm -f $@
	cp $< $@


#
# -- The $(X86_64-LOADER-IMG) is the bootable x86_64 image
#    -----------------------------------------------------
$(X86_64-LOADER-IMG): $(X86_64-LOADER-ELF)
	echo "X86_64-OBJCOPY:" $@
	mkdir -p $(dir $@)
	$(X86_64-LOADER-OBJCOPY) --only-keep-debug $< $@ && chmod -x $@


#
# -- Create the LOADER image
#    -----------------------
$(X86_64-LOADER-ELF): $(addprefix $(X86_64-LOADER-OBJ)/,$(X86_64-LOADER-O)) $(X86_64-LOADER-LIBS) $(X86_64-LOADER-LS)
	echo "X86_64-LD     :" $@
	mkdir -p $(dir $@)
	$(X86_64-LOADER-LD) -o $@ $(addprefix $(X86_64-LOADER-OBJ)/,$(X86_64-LOADER-O)) $(X86_64-LOADER-LIBS) $(GCC-LIB)
	

#
# -- Build the loader for the x86_64
#    -------------------------------
.PHONY: x86_64-loader
x86_64-loader: current-target


#
# -- Generic rule to make a .o file from the x86_64 source folder
#    -------------------------------------------------------------
$(X86_64-LOADER-OBJ)/%.o: $(X86_64-LOADER-SRC)/%.s 
	echo "X86_64-AS     :" $<
	mkdir -p $(dir $@)
	$(X86_64-LOADER-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the x86_64 source folder
#    -------------------------------------------------------------
$(X86_64-LOADER-OBJ)/%.d: $(X86_64-LOADER-SRC)/%.s
	echo "X86_64-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the x86_64 source folder
#    ------------------------------------------------------------
$(X86_64-LOADER-OBJ)/%.o: $(X86_64-LOADER-SRC)/%.c
	echo "X86_64-CC     :" $<
	mkdir -p $(dir $@)
	$(X86_64-LOADER-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the x86_64 source folder
#    ------------------------------------------------------------
$(I6X86_6486-LOADER-OBJ)/%.d: $(X86_64-LOADER-SRC)/%.c
	echo "X86_64-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- Generic rule to make a .o file from the loader source folder
#    ------------------------------------------------------------
$(X86_64-LOADER-OBJ)/%.o: $(LOADER-SRC)/%.s 
	echo "X86_64-AS     :" $<
	mkdir -p $(dir $@)
	$(X86_64-LOADER-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the loader source folder
#    ------------------------------------------------------------
$(X86_64-LOADER-OBJ)/%.d: $(LOADER-SRC)/%.s
	echo "X86_64-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the loader source folder
#    ------------------------------------------------------------
$(X86_64-LOADER-OBJ)/%.o: $(LOADER-SRC)/%.c
	echo "X86_64-CC     :" $<
	mkdir -p $(dir $@)
	$(X86_64-LOADER-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the loader source folder
#    ------------------------------------------------------------
$(X86_64-LOADER-OBJ)/%.d: $(LOADER-SRC)/%.c
	echo "X86_64-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- clean up
#    --------
.PHONY: x86_64-loader-clean
x86_64-loader-clean:
	echo "Cleaning x86_64-loader..."
	rm -fR $(X86_64-LOADER-OBJ)
	rm -fR $(dir $(X86_64-LOADER-IMG))
	rm -fR $(dir $(X86_64-LOADER-ELF))
	rm -fR $(dir $(X86_64-LOADER-SYS))
	
