#####################################################################################################################
##                                                                                                                 
##  src/kernel/src/rpi2b/Makefrag -- The make plans for the Raspberry Pi 2B version of the OS            
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  In order to keep the different Makefrag files from stepping on each other, all definitions will begin with 
##  'RPI2B-KERNEL-'.
##
## Within this Makefrag.mk and we need to make sure that the following make goals populate the 
## $(CURRENT-TARGET) macro:
## make
## make all
## make rpi2b
## make kernel
## make rpi2b-kernel
## make rpi2b-iso
## make run-rpi2b
##
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-03-26  Initial   0.0.0   ADCL  Initial version 
## 
#####################################################################################################################


#
# -- File Location definitions
#    -------------------------
RPI2B-KERNEL-SRC				:= $(KERNEL-SRC)/rpi2b
RPI2B-KERNEL-OBJ				:= $(KERNEL-OBJ)/rpi2b
RPI2B-KERNEL-BIN				:= $(KERNEL-BIN)/rpi2b
RPI2B-KERNEL-SYSROOT			:= $(KERNEL-SYSROOT)/rpi2b


#
# -- File Definitions
#    ----------------
RPI2B-KERNEL-S   				:= $(notdir $(wildcard $(RPI2B-KERNEL-SRC)/*.s) $(wildcard $(KERNEL-SRC)/*.s))
RPI2B-KERNEL-C   				:= $(notdir $(wildcard $(RPI2B-KERNEL-SRC)/*.c) $(wildcard $(KERNEL-SRC)/*.c))
RPI2B-KERNEL-O   				:= $(sort $(subst .s,.o,$(RPI2B-KERNEL-S)) $(subst .c,.o,$(RPI2B-KERNEL-C)))
RPI2B-KERNEL-D   				:= $(subst .o,.d,$(RPI2B-KERNEL-O))
RPI2B-KERNEL-LS  				:= $(RPI2B-KERNEL-SRC)/kernel.ld


#
# -- Target definitions
#    ------------------
RPI2B-KERNEL-IMG 				:= $(RPI2B-KERNEL-BIN)/kernel.img
RPI2B-KERNEL-ELF 				:= $(RPI2B-KERNEL-BIN)/kernel.elf
RPI2B-KERNEL-SYS 				:= $(subst $(RPI2B-KERNEL-BIN),$(RPI2B-KERNEL-SYSROOT)/boot,$(RPI2B-KERNEL-ELF))
RPI2B-GRUB-CNF					:= $(RPI2B-KERNEL-SYSROOT)/boot/grub/grub.cfg
RPI2B-ISO						+= iso/rpi2b.iso


#
# -- Some macros to help with finding the include files and building this component
#    ------------------------------------------------------------------------------
RPI2B-KERNEL-IA  				:= -Wa,-I -Wa,$(RPI2B-KERNEL-SRC) -Wa,-I -Wa,$(KERNEL-INC)
RPI2B-KERNEL-IC  				:= $(subst $(ASM-PARM),,$(RPI2B-KERNEL-IA))
RPI2B-KERNEL-PREFIX				:= arm-eabi


#
# -- Add to the global vars
#    ----------------------
ALL              				+= $(RPI2B-KERNEL-SYS)
CLEAN            				+= rpi2b-kernel-clean
DEPEND           				+= $(addprefix $(RPI2B-KERNEL-OBJ)/,$(RPI2B-KERNEL-D))


#
# -- Build Tool definitions
#    ----------------------
RPI2B-KERNEL-AS  				:= $(RPI2B-KERNEL-PREFIX)-gcc -mcpu=arm1176jzf-s -fpic -ffreestanding -x assembler-with-cpp \
		$(RPI2B-KERNEL-IA) -Wall -Werror -c
RPI2B-KERNEL-CC  				:= $(RPI2B-KERNEL-PREFIX)-gcc -mcpu=arm1176jzf-s -fpic -ffreestanding $(RPI2B-KERNEL-IC) -Wall -Werror -c
RPI2B-KERNEL-DEP 				:= $(RPI2B-KERNEL-PREFIX)-cpp -M -ffreestanding $(RPI2B-KERNEL-IC)
RPI2B-KERNEL-LD  				:= $(RPI2B-KERNEL-PREFIX)-gcc -T $(RPI2B-KERNEL-LS) -ffreestanding -O2 -nostdlib \
		-L~/opt/cross/lib/gcc/arm-eabi/6.3.0 -lgcc
RPI2B-KERNEL-LIBS  				:= 
RPI2B-KERNEL-OBJCOPY  			:= $(RPI2B-KERNEL-PREFIX)-objcopy


#
# -- Make sure that $(CURRENT-TARGET) is set up properly based on the goal
#    ---------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "")
CURRENT-TARGET					+= $(RPI2B-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "all")
CURRENT-TARGET					+= $(RPI2B-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "rpi2b")
CURRENT-TARGET					+= $(RPI2B-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "kernel")
CURRENT-TARGET					+= $(RPI2B-KERNEL-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "rpi2b-kernel")
CURRENT-TARGET					+= $(RPI2B-KERNEL-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "rpi2b-iso")
CURRENT-TARGET					+= $(RPI2B-ISO)
ISO								+= $(RPI2B-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "run-rpi2b")
CURRENT-TARGET					+= $(RPI2B-ISO) run-rpi2b-iso
ISO								+= $(RPI2B-KERNEL-SYS)
endif


#
# -- Make and run the kernel
#    -----------------------
.PHONY: run-rpi2b
run-rpi2b: $(RPI2B-ISO)
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel bin/rpi2b/kernel.elf


#
# -- The $(RPI2B-KERNEL-SYS) target, which will cover 3 of the 7 targets above
#    -------------------------------------------------------------------------
$(RPI2B-KERNEL-SYS): $(RPI2B-KERNEL-ELF)
	echo " RPI2B-SYSROOT:" $@
	mkdir -p $(dir $@)
	rm -f $@
	cp $< $@


#
# -- The $(RPI2B-KERNEL-IMG) is the bootable RPI2B image
#    -----------------------------------------------------
$(RPI2B-KERNEL-IMG): $(RPI2B-KERNEL-ELF)
	echo " RPI2B-OBJCOPY:" $@
	mkdir -p $(dir $@)
	$(RPI2B-KERNEL-OBJCOPY) --only-keep-debug $< $@ && chmod -x $@


#
# -- The CDROM image is needed by 2 of the 7 rules above
#    ---------------------------------------------------	
$(RPI2B-ISO): $(RPI2B-GRUB-CNF) $(RPI2B-KERNEL-ELF) $(ISO)
	echo " RPI2B-ISO    :" $@
	mkdir -p $(dir $@)
	grub2-mkrescue -o $(RPI2B-ISO) $(RPI2B-KERNEL-SYSROOT) 2> /dev/null


#
# -- Make the grub config files from this file
#    -----------------------------------------
$(RPI2B-GRUB-CNF): $(lastword $(MAKEFILE_LIST)) 
	echo " RPI2B-GRUB   :" $@
	mkdir -p $(dir $@)
	echo set timeout=3                    						>  $@
	echo set default=0	                  						>> $@
	echo menuentry \"Century \(Multiboot\)\" { 	      			>> $@
	echo   multiboot /boot/$(notdir $(RPI2B-KERNEL-ELF)) 		>> $@
	echo   boot							  						>> $@
	echo }								  						>> $@
	echo menuentry \"Century \(Multiboot2\)\" { 	  			>> $@
	echo   multiboot2 /boot/$(notdir $(RPI2B-KERNEL-ELF))		>> $@
	echo   boot							  						>> $@
	echo } 														>> $@


#
# -- Create the kernel image
#    -----------------------
$(RPI2B-KERNEL-ELF): $(addprefix $(RPI2B-KERNEL-OBJ)/,$(RPI2B-KERNEL-O)) $(RPI2B-KERNEL-LIBS) $(RPI2B-KERNEL-LS)
	echo " RPI2B-LD     :" $@
	mkdir -p $(dir $@)
	$(RPI2B-KERNEL-LD) -o $@ $(addprefix $(RPI2B-KERNEL-OBJ)/,$(RPI2B-KERNEL-O)) $(RPI2B-KERNEL-LIBS)
	


#
# -- We are going to make the OS for a rpi2b; build it all
#    -----------------------------------------------------
.PHONY: rpi2b
rpi2b: current-target


#
# -- Build the kernel for the rpi2b
#    ------------------------------
.PHONY: rpi2b-kernel
rpi2b-kernel: current-target


#
# -- Build the iso image
#    -------------------
.PHONY: rpi2b-iso
rpi2b-iso: current-target
	
	
#
# -- Generic rule to make a .o file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.o: $(RPI2B-KERNEL-SRC)/%.s 
	echo " RPI2B-AS     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-KERNEL-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.d: $(RPI2B-KERNEL-SRC)/%.s
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.o: $(RPI2B-KERNEL-SRC)/%.c
	echo " RPI2B-CC     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-KERNEL-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the rpi2b source folder
#    -----------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.d: $(RPI2B-KERNEL-SRC)/%.c
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- Generic rule to make a .o file from the kernel source folder
#    ------------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.o: $(KERNEL-SRC)/%.s 
	echo " RPI2B-AS     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-KERNEL-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the kernel source folder
#    ------------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.d: $(KERNEL-SRC)/%.s
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the kernel source folder
#    ------------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.o: $(KERNEL-SRC)/%.c
	echo " RPI2B-CC     :" $<
	mkdir -p $(dir $@)
	$(RPI2B-KERNEL-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the kerpen source folder
#    ------------------------------------------------------------
$(RPI2B-KERNEL-OBJ)/%.d: $(KERNEL-SRC)/%.c
	echo " RPI2B-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(RPI2B-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(RPI2B-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- clean up
#    --------
.PHONY: rpi2b-kernel-clean
rpi2b-kernel-clean:
	echo "Cleaning rpi2b-kernel..."
	rm -fR $(RPI2B-KERNEL-OBJ)
	rm -fR $(dir $(RPI2B-KERNEL-IMG))
	rm -fR $(dir $(RPI2B-KERNEL-ELF))
	rm -fR $(dir $(RPI2B-KERNEL-SYS))
	