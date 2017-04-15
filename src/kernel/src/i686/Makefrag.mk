#####################################################################################################################
##                                                                                                                 
##  src/kernel/src/i686/Makefrag -- The make plans for the i686 (32-bit) version of the OS
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  In order to keep the different Makefrag files from stepping on each other, all definitions will begin with 
##  'I686-KERNEL-'.
##
## Within this Makefrag.mk and we need to make sure that the following make goals populate the 
## $(CURRENT-TARGET) macro:
## make
## make all
## make i686
## make kernel
## make i686-kernel
## make i686-iso
## make run-i686
##
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-07  Initial   0.0.0   ADCL  Initial version 
## 
#####################################################################################################################




#
# -- File Location definitions
#    -------------------------
I686-KERNEL-SRC					:= $(KERNEL-SRC)/i686
I686-KERNEL-OBJ					:= $(KERNEL-OBJ)/i686
I686-KERNEL-BIN					:= $(KERNEL-BIN)/i686
I686-KERNEL-SYSROOT				:= $(KERNEL-SYSROOT)/i686


#
# -- File Definitions
#    ----------------
I686-KERNEL-S   				:= $(notdir $(wildcard $(I686-KERNEL-SRC)/*.s) $(wildcard $(KERNEL-SRC)/*.s))
I686-KERNEL-C   				:= $(notdir $(wildcard $(I686-KERNEL-SRC)/*.c) $(wildcard $(KERNEL-SRC)/*.c))
I686-KERNEL-O   				:= $(sort $(subst .s,.o,$(I686-KERNEL-S)) $(subst .c,.o,$(I686-KERNEL-C)))
I686-KERNEL-D   				:= $(subst .o,.d,$(I686-KERNEL-O))
I686-KERNEL-LS  				:= $(I686-KERNEL-SRC)/kernel.ld


#
# -- Target definitions
#    ------------------
I686-KERNEL-IMG 				:= $(I686-KERNEL-BIN)/kernel.img
I686-KERNEL-ELF 				:= $(I686-KERNEL-BIN)/kernel.elf
I686-KERNEL-SYS 				:= $(subst $(I686-KERNEL-BIN),$(I686-KERNEL-SYSROOT)/boot,$(I686-KERNEL-ELF))
I686-GRUB-CNF					:= $(I686-KERNEL-SYSROOT)/boot/grub/grub.cfg
I686-ISO						+= iso/i686.iso


#
# -- Some macros to help with finding the include files and building this component
#    ------------------------------------------------------------------------------
I686-KERNEL-IA  				:= -Wa,-I -Wa,$(I686-KERNEL-SRC) -Wa,-I -Wa,$(KERNEL-INC)
I686-KERNEL-IC  				:= $(subst $(ASM-PARM),,$(I686-KERNEL-IA))
I686-KERNEL-PREFIX				:= i686-elf


#
# -- Add to the global vars
#    ----------------------
ALL              				+= $(I686-KERNEL-SYS)
CLEAN            				+= i686-kernel-clean
DEPEND           				+= $(addprefix $(I686-KERNEL-OBJ)/,$(I686-KERNEL-D))


#
# -- Build Tool definitions
#    ----------------------
I686-KERNEL-AS  				:= $(I686-KERNEL-PREFIX)-gcc -fpic -ffreestanding -x assembler-with-cpp \
		$(I686-KERNEL-IA) -Wall -Werror -c
I686-KERNEL-CC  				:= $(I686-KERNEL-PREFIX)-gcc -fpic -ffreestanding $(I686-KERNEL-IC) -Wall -Werror -c
I686-KERNEL-DEP 				:= $(I686-KERNEL-PREFIX)-cpp -M -ffreestanding $(I686-KERNEL-IC)
I686-KERNEL-LD  				:= $(I686-KERNEL-PREFIX)-gcc -T $(I686-KERNEL-LS) -ffreestanding -O2 -nostdlib \
		-L~/opt/cross/lib/gcc/i686-elf/6.3.0 -lgcc
I686-KERNEL-LIBS  				:= 
I686-KERNEL-OBJCOPY  			:= $(I686-KERNEL-PREFIX)-objcopy


#
# -- Make sure that $(CURRENT-TARGET) is set up properly based on the goal
#    ---------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "")
CURRENT-TARGET					+= $(I686-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "all")
CURRENT-TARGET					+= $(I686-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "i686")
CURRENT-TARGET					+= $(I686-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "kernel")
CURRENT-TARGET					+= $(I686-KERNEL-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "i686-kernel")
CURRENT-TARGET					+= $(I686-KERNEL-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "i686-iso")
CURRENT-TARGET					+= $(I686-ISO)
ISO								+= $(I686-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "run-i686")
CURRENT-TARGET					+= $(I686-ISO) run-i686-iso
ISO								+= $(I686-KERNEL-SYS)
endif


#
# -- run-i686-iso is the pinacle of this architecture
#    ------------------------------------------------
.PHONY: run-i686
run-i686: $(I686-ISO)
	qemu-system-i386 -m 256 -serial stdio -cdrom $<


#
# -- The $(I686-KERNEL-SYS) target, which will cover 3 of the 7 targets above
#    ------------------------------------------------------------------------
$(I686-KERNEL-SYS): $(I686-KERNEL-ELF)
	echo "  I686-SYSROOT:" $@
	mkdir -p $(dir $@)
	rm -f $@
	cp $< $@


#
# -- The $(I686-KERNEL-IMG) is the bootable i686 image
#    -------------------------------------------------
$(I686-KERNEL-IMG): $(I686-KERNEL-ELF)
	echo "  I686-OBJCOPY:" $@
	mkdir -p $(dir $@)
	$(I686-KERNEL-OBJCOPY) --only-keep-debug $< $@ && chmod -x $@


#
# -- The CDROM image is needed by 2 of the 7 rules above
#    ---------------------------------------------------	
$(I686-ISO): $(I686-GRUB-CNF) $(I686-KERNEL-ELF) $(ISO)
	echo "  I686-ISO    :" $@
	mkdir -p $(dir $@)
	grub2-mkrescue -o $(I686-ISO) $(I686-KERNEL-SYSROOT) 2> /dev/null


#
# -- Make the grub config files from this file
#    -----------------------------------------
$(I686-GRUB-CNF): $(lastword $(MAKEFILE_LIST)) 
	echo "  I686-GRUB   :" $@
	mkdir -p $(dir $@)
	echo set timeout=3                    						>  $@
	echo set default=0	                  						>> $@
	echo menuentry \"Century \(Multiboot\)\" { 	      			>> $@
	echo   multiboot /boot/$(notdir $(I686-LOADER-ELF)) 		>> $@
	echo   boot							  						>> $@
	echo }								  						>> $@
	echo menuentry \"Century \(Multiboot2\)\" { 	  			>> $@
	echo   multiboot2 /boot/$(notdir $(I686-LOADER-ELF))		>> $@
	echo   boot							  						>> $@
	echo } 														>> $@


#
# -- Create the kernel image
#    -----------------------
$(I686-KERNEL-ELF): $(addprefix $(I686-KERNEL-OBJ)/,$(I686-KERNEL-O)) $(I686-KERNEL-LIBS) $(I686-KERNEL-LS)
	echo "  I686-LD     :" $@
	mkdir -p $(dir $@)
	$(I686-KERNEL-LD) -o $@ $(addprefix $(I686-KERNEL-OBJ)/,$(I686-KERNEL-O)) $(I686-KERNEL-LIBS)
	

#
# -- We are going to make the OS for a i686; build it all
#    ----------------------------------------------------
.PHONY: i686
i686: current-target


#
# -- Build the kernel for the i686
#    -----------------------------
.PHONY: i686-kernel
i686-kernel: current-target


#
# -- Build the iso image
#    -------------------
.PHONY: i686-iso
i686-iso: current-target


#
# -- Generic rule to make a .o file from the i686 source folder
#    -----------------------------------------------------------
$(I686-KERNEL-OBJ)/%.o: $(I686-KERNEL-SRC)/%.s 
	echo "  I686-AS     :" $<
	mkdir -p $(dir $@)
	$(I686-KERNEL-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the i686 source folder
#    -----------------------------------------------------------
$(I686-KERNEL-OBJ)/%.d: $(I686-KERNEL-SRC)/%.s
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the i686 source folder
#    -----------------------------------------------------------
$(I686-KERNEL-OBJ)/%.o: $(I686-KERNEL-SRC)/%.c
	echo "  I686-CC     :" $<
	mkdir -p $(dir $@)
	$(I686-KERNEL-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the i686 source folder
#    -----------------------------------------------------------
$(I686-KERNEL-OBJ)/%.d: $(I686-KERNEL-SRC)/%.c
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- Generic rule to make a .o file from the kernel source folder
#    ------------------------------------------------------------
$(I686-KERNEL-OBJ)/%.o: $(KERNEL-SRC)/%.s 
	echo "  I686-AS     :" $<
	mkdir -p $(dir $@)
	$(I686-KERNEL-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the kernel source folder
#    ------------------------------------------------------------
$(I686-KERNEL-OBJ)/%.d: $(KERNEL-SRC)/%.s
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the kernel source folder
#    ------------------------------------------------------------
$(I686-KERNEL-OBJ)/%.o: $(KERNEL-SRC)/%.c
	echo "  I686-CC     :" $<
	mkdir -p $(dir $@)
	$(I686-KERNEL-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the kernel source folder
#    ------------------------------------------------------------
$(I686-KERNEL-OBJ)/%.d: $(KERNEL-SRC)/%.c
	echo "  I686-DEPEND :" $<
	mkdir -p $(dir $@)
	rm -f $@
	$(I686-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(I686-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- clean up
#    --------
.PHONY: i686-kernel-clean
i686-kernel-clean:
	echo "Cleaning i686-kernel..."
	rm -fR $(I686-KERNEL-OBJ)
	rm -fR $(dir $(I686-KERNEL-IMG))
	rm -fR $(dir $(I686-KERNEL-ELF))
	rm -fR $(dir $(I686-KERNEL-SYS))
	