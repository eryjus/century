#####################################################################################################################
##                                                                                                                 
##  loader i686/Makefrag.mk -- This file controls making the loader for the i686 architecture (next after multioot)
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
##  make i686
##  make loader
##  make i686-loader
##  make i686-iso
##  make run-i686
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
I686-LOADER-SRC					:= $(LOADER-SRC)/i686
I686-LOADER-OBJ					:= $(LOADER-OBJ)/i686
I686-LOADER-BIN					:= $(LOADER-BIN)/i686
I686-LOADER-SYSROOT				:= $(LOADER-SYSROOT)/i686


#
# -- File Definitions
#    ----------------
I686-LOADER-S   				:= $(notdir $(wildcard $(I686-LOADER-SRC)/*.s) $(wildcard $(LOADER-SRC)/*.s))
I686-LOADER-C   				:= $(notdir $(wildcard $(I686-LOADER-SRC)/*.c) $(wildcard $(LOADER-SRC)/*.c))
I686-LOADER-O   				:= $(sort $(subst .s,.o,$(I686-LOADER-S)) $(subst .c,.o,$(I686-LOADER-C)))
I686-LOADER-D   				:= $(subst .o,.d,$(I686-LOADER-O))
I686-LOADER-LS  				:= $(I686-LOADER-SRC)/loader.ld


#
# -- Target definitions
#    ------------------
I686-LOADER-IMG 				:= $(I686-LOADER-BIN)/loader.img
I686-LOADER-ELF 				:= $(I686-LOADER-BIN)/loader.elf
I686-LOADER-SYS 				:= $(subst $(I686-LOADER-BIN),$(I686-LOADER-SYSROOT)/boot,$(I686-LOADER-ELF))


#
# -- Some macros to help with finding the include files and building this component
#    ------------------------------------------------------------------------------
I686-LOADER-IA  				:= -Wa,-I -Wa,$(I686-LOADER-SRC) -Wa,-I -Wa,$(LOADER-INC)
I686-LOADER-IC  				:= $(subst $(ASM-PARM),,$(I686-LOADER-IA))
I686-LOADER-PREFIX				:= i686-elf


#
# -- Add to the global vars
#    ----------------------
ALL              				+= $(I686-LOADER-SYS)
CLEAN            				+= i686-loader-clean
DEPEND           				+= $(addprefix $(I686-LOADER-OBJ)/,$(I686-LOADER-D))


#
# -- Build Tool definitions
#    ----------------------
I686-LOADER-AS  				:= $(I686-LOADER-PREFIX)-gcc -fpic -ffreestanding -x assembler-with-cpp \
		$(I686-LOADER-IA) -Wall -Werror -c
I686-LOADER-CC  				:= $(I686-LOADER-PREFIX)-gcc -fpic -ffreestanding $(I686-LOADER-IC) -Wall -Werror -c
I686-LOADER-DEP 				:= $(I686-LOADER-PREFIX)-cpp -M -ffreestanding $(I686-LOADER-IC)
I686-LOADER-LD  				:= $(I686-LOADER-PREFIX)-gcc -T $(I686-LOADER-LS) -ffreestanding -O2 -nostdlib 
I686-LOADER-LIBS  				:= 
I686-LOADER-OBJCOPY  			:= $(I686-LOADER-PREFIX)-objcopy


#
# -- Make sure that $(CURRENT-TARGET) is set up properly based on the goal
#    ---------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "")
CURRENT-TARGET					+= $(I686-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "all")
CURRENT-TARGET					+= $(I686-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "i686")
CURRENT-TARGET					+= $(I686-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "loader")
CURRENT-TARGET					+= $(I686-LOADER-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "i686-loader")
CURRENT-TARGET					+= $(I686-LOADER-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "i686-iso")
ISO								+= $(I686-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "run-i686")
ISO								+= $(I686-LOADER-SYS)
endif


#
# -- The $(I686-LOADER-SYS) target, which will cover 3 of the 7 targets above
#    ------------------------------------------------------------------------
$(I686-LOADER-SYS): $(I686-LOADER-ELF)
	echo "  I686-SYSROOT:" $@
	mkdir -p $(dir $@)
	rm -f $@
	cp $< $@


#
# -- The $(I686-LOADER-IMG) is the bootable i686 image
#    -------------------------------------------------
$(I686-LOADER-IMG): $(I686-LOADER-ELF)
	echo "  I686-OBJCOPY:" $@
	mkdir -p $(dir $@)
	$(I686-LOADER-OBJCOPY) --only-keep-debug $< $@ && chmod -x $@


#
# -- Create the LOADER image
#    -----------------------
$(I686-LOADER-ELF): $(addprefix $(I686-LOADER-OBJ)/,$(I686-LOADER-O)) $(I686-LOADER-LIBS) $(I686-LOADER-LS)
	echo "  I686-LD     :" $@
	mkdir -p $(dir $@)
	$(I686-LOADER-LD) -o $@ $(addprefix $(I686-LOADER-OBJ)/,$(I686-LOADER-O)) $(I686-LOADER-LIBS) $(GCC-LIB)
	

#
# -- Build the loader for the i686
#    -----------------------------
.PHONY: i686-loader
i686-loader: current-target


#
# -- Generic rule to make a .o file from the i686 source folder
#    -----------------------------------------------------------
$(I686-LOADER-OBJ)/%.o: $(I686-LOADER-SRC)/%.s 
	echo "  I686-AS     :" $<
	mkdir -p $(dir $@)
	$(I686-LOADER-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the i686 source folder
#    -----------------------------------------------------------
$(I686-LOADER-OBJ)/%.d: $(I686-LOADER-SRC)/%.s
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the i686 source folder
#    -----------------------------------------------------------
$(I686-LOADER-OBJ)/%.o: $(I686-LOADER-SRC)/%.c
	echo "  I686-CC     :" $<
	mkdir -p $(dir $@)
	$(I686-LOADER-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the i686 source folder
#    -----------------------------------------------------------
$(I686-LOADER-OBJ)/%.d: $(I686-LOADER-SRC)/%.c
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- Generic rule to make a .o file from the loader source folder
#    ------------------------------------------------------------
$(I686-LOADER-OBJ)/%.o: $(LOADER-SRC)/%.s 
	echo "  I686-AS     :" $<
	mkdir -p $(dir $@)
	$(I686-LOADER-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the loader source folder
#    ------------------------------------------------------------
$(I686-LOADER-OBJ)/%.d: $(LOADER-SRC)/%.s
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the loader source folder
#    ------------------------------------------------------------
$(I686-LOADER-OBJ)/%.o: $(LOADER-SRC)/%.c
	echo "  I686-CC     :" $<
	mkdir -p $(dir $@)
	$(I686-LOADER-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the loader source folder
#    ------------------------------------------------------------
$(I686-LOADER-OBJ)/%.d: $(LOADER-SRC)/%.c
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- clean up
#    --------
.PHONY: i686-loader-clean
i686-loader-clean:
	echo "Cleaning i686-loader..."
	rm -fR $(I686-LOADER-OBJ)
	rm -fR $(dir $(I686-LOADER-IMG))
	rm -fR $(dir $(I686-LOADER-ELF))
	rm -fR $(dir $(I686-LOADER-SYS))
	
