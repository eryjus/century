#####################################################################################################################
##                                                                                                                 
##  src/kernel/src/x86_64/Makefrag -- The make plans for the x86_64 (64-bit) version of the OS
##
##        Copyright (c)  2017 -- Adam Clark
##
##        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
##        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
##        you can do whatever you want with this stuff. If we meet some day, and you
##        think this stuff is worth it, you can buy me a beer in return.
##
##  In order to keep the different Makefrag files from stepping on each other, all definitions will begin with 
##  'X86_64-KERNEL-'.
##
## Within this Makefrag.mk and we need to make sure that the following make goals populate the 
## $(CURRENT-TARGET) macro:
## make
## make all
## make x86_64
## make kernel
## make x86_64-kernel
## make x86_64-iso
## make run-x86_64
##
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-04-09  Initial   0.0.0   ADCL  Initial version 
## 
#####################################################################################################################


#
# -- File Location definitions
#    -------------------------
X86_64-KERNEL-SRC				:= $(KERNEL-SRC)/x86_64
X86_64-KERNEL-OBJ				:= $(KERNEL-OBJ)/x86_64
X86_64-KERNEL-BIN				:= $(KERNEL-BIN)/x86_64
X86_64-KERNEL-SYSROOT			:= $(KERNEL-SYSROOT)/x86_64


#
# -- File Definitions
#    ----------------
X86_64-KERNEL-S   				:= $(notdir $(wildcard $(X86_64-KERNEL-SRC)/*.s) $(wildcard $(KERNEL-SRC)/*.s))
X86_64-KERNEL-C   				:= $(notdir $(wildcard $(X86_64-KERNEL-SRC)/*.c) $(wildcard $(KERNEL-SRC)/*.c))
X86_64-KERNEL-O   				:= $(sort $(subst .s,.o,$(X86_64-KERNEL-S)) $(subst .c,.o,$(X86_64-KERNEL-C)))
X86_64-KERNEL-D   				:= $(subst .o,.d,$(X86_64-KERNEL-O))
X86_64-KERNEL-LS  				:= $(X86_64-KERNEL-SRC)/kernel.ld


#
# -- Target definitions
#    ------------------
X86_64-KERNEL-IMG 				:= $(X86_64-KERNEL-BIN)/kernel.img
X86_64-KERNEL-ELF 				:= $(X86_64-KERNEL-BIN)/kernel.elf
X86_64-KERNEL-SYS 				:= $(subst $(X86_64-KERNEL-BIN),$(X86_64-KERNEL-SYSROOT)/boot,$(X86_64-KERNEL-ELF))
X86_64-GRUB-CNF					:= $(X86_64-KERNEL-SYSROOT)/boot/grub/grub.cfg
X86_64-ISO						+= iso/x86_64.iso


#
# -- Some macros to help with finding the include files and building this component
#    ------------------------------------------------------------------------------
X86_64-KERNEL-IA  				:= -Wa,-I -Wa,$(X86_64-KERNEL-SRC) -Wa,-I -Wa,$(KERNEL-INC)
X86_64-KERNEL-IC  				:= $(subst $(ASM-PARM),,$(X86_64-KERNEL-IA))
X86_64-KERNEL-PREFIX			:= x86_64-elf


#
# -- Add to the global vars
#    ----------------------
ALL             				+= $(X86_64-KERNEL-SYS)
CLEAN            				+= x86_64-kernel-clean
DEPEND           				+= $(addprefix $(X86_64-KERNEL-OBJ)/,$(X86_64-KERNEL-D))


#
# -- Build Tool definitions
#    ----------------------
X86_64-KERNEL-AS  				:= $(X86_64-KERNEL-PREFIX)-gcc -fpic -ffreestanding -x assembler-with-cpp \
		$(X86_64-KERNEL-IA) -Wall -Werror -c
X86_64-KERNEL-CC  				:= $(X86_64-KERNEL-PREFIX)-gcc -fpic -ffreestanding $(X86_64-KERNEL-IC) -Wall -Werror -c
X86_64-KERNEL-DEP 				:= $(X86_64-KERNEL-PREFIX)-cpp -M -ffreestanding $(X86_64-KERNEL-IC)
X86_64-KERNEL-LD 				:= $(X86_64-KERNEL-PREFIX)-gcc -T $(X86_64-KERNEL-LS) -ffreestanding -O2 -nostdlib \
		-L~/opt/cross/lib/gcc/x86_64/6.3.0 -lgcc -z max-page-size=0x800
X86_64-KERNEL-LIBS  			:= 
X86_64-KERNEL-OBJCOPY  			:= $(X86_64-KERNEL-PREFIX)-objcopy


#
# -- Make sure that $(CURRENT-TARGET) is set up properly based on the goal
#    ---------------------------------------------------------------------
ifeq ("$(MAKECMDGOALS)", "")
CURRENT-TARGET					+= $(X86_64-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "all")
CURRENT-TARGET					+= $(X86_64-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "x86_64")
CURRENT-TARGET					+= $(X86_64-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "kernel")
CURRENT-TARGET					+= $(X86_64-KERNEL-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "x86_64-kernel")
CURRENT-TARGET					+= $(X86_64-KERNEL-IMG)
endif
ifeq ("$(MAKECMDGOALS)", "x86_64-iso")
CURRENT-TARGET					+= $(X86_64-ISO)
ISO								+= $(X86_64-KERNEL-SYS)
endif
ifeq ("$(MAKECMDGOALS)", "run-x86_64")
CURRENT-TARGET					+= $(X86_64-ISO) run-x86_64-iso
ISO								+= $(X86_64-KERNEL-SYS)
endif


#
# -- run-x86-64-iso is the pinacle of this architecture
#    --------------------------------------------------
.PHONY: run-x86_64
run-x86_64: $(X86_64-ISO)
	qemu-system-x86_64 -m 256 -serial stdio -cdrom $<


#
# -- The $(X86_64-KERNEL-SYS) target, which will cover 3 of the 7 targets above
#    --------------------------------------------------------------------------
$(X86_64-KERNEL-SYS): $(X86_64-KERNEL-ELF)
	echo "X86_64-SYSROOT:" $(notdir $@)	
	mkdir -p $(dir $@)
	rm -f $@
	cp $< $@


#
# -- The $(X86_64-KERNEL-IMG) is the bootable x86_64 image
#    -----------------------------------------------------
$(X86_64-KERNEL-IMG): $(X86_64-KERNEL-ELF)
	echo "X86_64-OBJCOPY:" $(notdir $@)	
	mkdir -p $(dir $@)
	$(X86_64-KERNEL-OBJCOPY) --only-keep-debug $< $@ && chmod -x $@


#
# -- The CDROM image is needed by 2 of the 7 rules above
#    ---------------------------------------------------	
$(X86_64-ISO): $(X86_64-GRUB-CNF) $(X86_64-KERNEL-ELF) $(ISO)
	echo "X86_64-ISO    " $@...
	mkdir -p $(dir $@)
	grub2-mkrescue -o $(X86_64-ISO) $(X86_64-KERNEL-SYSROOT)


#
# -- Make the grub config files from this file
#    -----------------------------------------
$(X86_64-GRUB-CNF): $(lastword $(MAKEFILE_LIST)) 
	echo "X86_64-GRUB"
	mkdir -p $(dir $@)
	echo set timeout=3                    						>  $@
	echo set default=0	                  						>> $@
	echo menuentry \"Century \(Multiboot\)\" { 	      			>> $@
	echo   multiboot /boot/$(notdir $(X86_64-KERNEL-ELF)) 		>> $@
	echo   boot							  						>> $@
	echo }								  						>> $@
	echo menuentry \"Century \(Multiboot2\)\" { 	  			>> $@
	echo   multiboot2 /boot/$(notdir $(X86_64-KERNEL-ELF))		>> $@
	echo   boot							  						>> $@
	echo } 														>> $@


#
# -- Create the kernel image
#    -----------------------
$(X86_64-KERNEL-ELF): $(addprefix $(X86_64-KERNEL-OBJ)/,$(X86_64-KERNEL-O)) $(X86_64-KERNEL-LIBS) $(X86_64-KERNEL-LS)
	echo "X86_64-LD     :" $(notdir $@)
	mkdir -p $(dir $@)
	$(X86_64-KERNEL-LD) -o $@ $(addprefix $(X86_64-KERNEL-OBJ)/,$(X86_64-KERNEL-O)) $(X86_64-KERNEL-LIBS)
	

#
# -- We are going to make the OS for a x86_64; build it all
#    ------------------------------------------------------
.PHONY: x86_64
x86_64: current-target


#
# -- Build the kernel for the x86_64
#    -------------------------------
.PHONY: x86_64-kernel
x86_64-kernel: current-target


#
# -- Build the iso image
#    -------------------
.PHONY: x86_64-iso
x86_64-iso: current-target

	
#
# -- Generic rule to make a .o file from the x86_64 source folder
#    ------------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.o: $(X86_64-KERNEL-SRC)/%.s 
	echo "X86_64-AS     :" $(notdir $<)
	mkdir -p $(dir $@)
	$(X86_64-KERNEL-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the x86_64 source folder
#    ------------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.d: $(X86_64-KERNEL-SRC)/%.s
	echo "X86_64-DEPEND :" $(notdir $<)
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the x86_64 source folder
#    ------------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.o: $(X86_64-KERNEL-SRC)/%.c
	echo "X86_64-CC     :" $(notdir $<)
	mkdir -p $(dir $@)
	$(X86_64-KERNEL-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the x86_64 source folder
#    -----------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.d: $(X86_64-KERNEL-SRC)/%.c
	echo "X86_64-DEPEND :" $(notdir $<)
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- Generic rule to make a .o file from the kernel source folder
#    ------------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.o: $(KERNEL-SRC)/%.s 
	echo "X86_64-AS     :" $(notdir $<)
	mkdir -p $(dir $@)
	$(X86_64-KERNEL-AS) -o $@ $<

	
#
# -- Generic rule to make a .d file from the kernel source folder
#    ------------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.d: $(KERNEL-SRC)/%.s
	echo "X86_64-DEPEND :" $(notdir $<)
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$


#
# -- Generic rule to make a .o file from the kernel source folder
#    ------------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.o: $(KERNEL-SRC)/%.c
	echo "X86_64-CC     :" $(notdir $<)
	mkdir -p $(dir $@)
	$(X86_64-KERNEL-CC) -o $@ $<

	
#
# -- Generic rule to make a .d file from the kernel source folder
#    ------------------------------------------------------------
$(X86_64-KERNEL-OBJ)/%.d: $(KERNEL-SRC)/%.c
	echo "X86_64-DEPEND :" $(notdir $<)
	mkdir -p $(dir $@)
	rm -f $@
	$(X86_64-KERNEL-DEP)  $<  > $@.$$$$;													\
	sed 's,\($*\)\.o[ :]*,$(X86_64-KERNEL-OBJ)/\1.o $@ : ,g' < $@.$$$$ > $@;				\
	rm -f $@.$$$$
	

#
# -- clean up
#    --------
.PHONY: x86_64-kernel-clean
x86_64-kernel-clean:
	echo "Cleaning x86_64-kernel..."
	rm -fR $(X86_64-KERNEL-OBJ)
	rm -fR $(dir $(X86_64-KERNEL-IMG))
	rm -fR $(dir $(X86_64-KERNEL-ELF))
	rm -fR $(dir $(X86_64-KERNEL-SYS))
	