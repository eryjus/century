#####################################################################################################################
##                                                                                                                 
##  loader rpi2b/Makefrag -- The make plans for the Raspberry Pi 2B version of the OS            
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  In order to keep the different Makefrag files from stepping on each other, all definitions will begin with 
##  'RPI2B-LOADER-'.
##
## Within this Makefrag.mk and we need to make sure that the following make goals populate the 
## $(CURRENT-TARGET) macro:
## make
## make all
## make rpi2b
## make loader
## make rpi2b-loader
## make rpi2b-iso
## make run-rpi2b
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
RPI2B-LOADER-SRC				:= $(LOADER-SRC)/rpi2b
RPI2B-LOADER-OBJ				:= $(LOADER-OBJ)/rpi2b
RPI2B-LOADER-BIN				:= $(LOADER-BIN)/rpi2b
RPI2B-LOADER-SYSROOT			:= $(LOADER-SYSROOT)/rpi2b


#
# -- File Definitions
#    ----------------
RPI2B-LOADER-S   				:= $(notdir $(wildcard $(RPI2B-LOADER-SRC)/*.s) $(wildcard $(LOADER-SRC)/*.s))
RPI2B-LOADER-C   				:= $(notdir $(wildcard $(RPI2B-LOADER-SRC)/*.c) $(wildcard $(LOADER-SRC)/*.c))
RPI2B-LOADER-O   				:= $(sort $(subst .s,.o,$(RPI2B-LOADER-S)) $(subst .c,.o,$(RPI2B-LOADER-C)))
RPI2B-LOADER-D   				:= $(subst .o,.d,$(RPI2B-LOADER-O))
RPI2B-LOADER-LS  				:= $(RPI2B-LOADER-SRC)/loader.ld


#
# -- Target definitions
#    ------------------
RPI2B-LOADER-IMG 				:= $(RPI2B-LOADER-BIN)/loader.img
RPI2B-LOADER-ELF 				:= $(RPI2B-LOADER-BIN)/loader.elf
RPI2B-LOADER-SYS 				:= $(subst $(RPI2B-LOADER-BIN),$(RPI2B-LOADER-SYSROOT)/boot,$(RPI2B-LOADER-ELF))
RPI2B-GRUB-CNF					:= $(RPI2B-LOADER-SYSROOT)/boot/grub/grub.cfg
RPI2B-ISO						+= iso/rpi2b.iso


#
# -- Some macros to help with finding the include files and building this component
#    ------------------------------------------------------------------------------
RPI2B-LOADER-IA  				:= -Wa,-I -Wa,$(RPI2B-LOADER-SRC) -Wa,-I -Wa,$(LOADER-INC)
RPI2B-LOADER-IC  				:= $(subst $(ASM-PARM),,$(RPI2B-LOADER-IA))
RPI2B-LOADER-PREFIX				:= arm-eabi


#
# -- Add to the global vars
#    ----------------------
ALL              				+= $(RPI2B-LOADER-SYS)
CLEAN            				+= rpi2b-loader-clean
DEPEND           				+= $(addprefix $(RPI2B-LOADER-OBJ)/,$(RPI2B-LOADER-D))


#
# -- Build Tool definitions
#    ----------------------
RPI2B-LOADER-AS  				:= $(RPI2B-LOADER-PREFIX)-gcc -mcpu=arm1176jzf-s -fpic -ffreestanding -x assembler-with-cpp \
		$(RPI2B-LOADER-IA) -Wall -Werror -c
RPI2B-LOADER-CC  				:= $(RPI2B-LOADER-PREFIX)-gcc -mcpu=arm1176jzf-s -fpic -ffreestanding $(RPI2B-LOADER-IC) \
		-Wall -Werror -c
RPI2B-LOADER-DEP 				:= $(RPI2B-LOADER-PREFIX)-cpp -M -ffreestanding $(RPI2B-LOADER-IC)
RPI2B-LOADER-LD  				:= $(RPI2B-LOADER-PREFIX)-gcc -T $(RPI2B-LOADER-LS) -ffreestanding -O2 -nostdlib \
		-z max-page-size=0x1000 
RPI2B-LOADER-LIBS  				:= 
RPI2B-LOADER-OBJCOPY  			:= $(RPI2B-LOADER-PREFIX)-objcopy


#
# -- Make sure that $(CURRENT-TARGET) is set up properly based on the goal
#    ---------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "")
CURRENT-TARGET					+= $(RPI2B-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "all")
CURRENT-TARGET					+= $(RPI2B-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "rpi2b")
CURRENT-TARGET					+= $(RPI2B-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "loader")
CURRENT-TARGET					+= $(RPI2B-LOADER-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "rpi2b-loader")
CURRENT-TARGET					+= $(RPI2B-LOADER-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "rpi2b-iso")
ISO								+= $(RPI2B-LOADER-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "run-rpi2b")
ISO								+= $(RPI2B-LOADER-SYS)
endif


#
# -- The $(RPI2B-LOADER-SYS) target, which will cover 3 of the 7 targets above
#    -------------------------------------------------------------------------
$(RPI2B-LOADER-SYS): $(RPI2B-LOADER-ELF)
	echo " RPI2B-SYSROOT:" $@
	mkdir -p $(dir $@)
	rm -f $@
	cp $< $@


#
# -- The $(RPI2B-LOADER-IMG) is the bootable RPI2B image
#    -----------------------------------------------------
$(RPI2B-LOADER-IMG): $(RPI2B-LOADER-ELF)
	echo " RPI2B-OBJCOPY:" $@
	mkdir -p $(dir $@)
	$(RPI2B-LOADER-OBJCOPY) --only-keep-debug $< $@ && chmod -x $@


#
# -- Create the loader image
#    -----------------------
$(RPI2B-LOADER-ELF): $(addprefix $(RPI2B-LOADER-OBJ)/,$(RPI2B-LOADER-O)) $(RPI2B-LOADER-LIBS) $(RPI2B-LOADER-LS)
	echo " RPI2B-LD     :" $@
	mkdir -p $(dir $@)
	$(RPI2B-LOADER-LD) -o $@ $(addprefix $(RPI2B-LOADER-OBJ)/,$(RPI2B-LOADER-O)) $(RPI2B-LOADER-LIBS) $(GCC-LIB)
	


#
# -- Build the loader for the rpi2b
#    ------------------------------
.PHONY: rpi2b-loader
rpi2b-loader: current-target


#
# -- Generic rule to make a .o file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.o: $(RPI2B-LOADER-SRC)/%.s 
	echo " RPI2B-AS     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-LOADER-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.d: $(RPI2B-LOADER-SRC)/%.s
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.o: $(RPI2B-LOADER-SRC)/%.c
	echo " RPI2B-CC     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-LOADER-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.d: $(RPI2B-LOADER-SRC)/%.c
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- Generic rule to make a .o file from the loader source folder
#    ------------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.o: $(LOADER-SRC)/%.s 
	echo " RPI2B-AS     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-LOADER-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the loader source folder
#    ------------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.d: $(LOADER-SRC)/%.s
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the loader source folder
#    ------------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.o: $(LOADER-SRC)/%.c
	echo " RPI2B-CC     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-LOADER-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the kerpen source folder
#    ------------------------------------------------------------
$(RPI2B-LOADER-OBJ)/%.d: $(LOADER-SRC)/%.c
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-LOADER-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-LOADER-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- clean up
#    --------
.PHONY: rpi2b-loader-clean
rpi2b-loader-clean:
	echo "Cleaning rpi2b-loader..."
	rm -fR $(RPI2B-LOADER-OBJ)
	rm -fR $(dir $(RPI2B-LOADER-IMG))
	rm -fR $(dir $(RPI2B-LOADER-ELF))
	rm -fR $(dir $(RPI2B-LOADER-SYS))
	