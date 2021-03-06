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
##    +- modules        // source files common to all architectures
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
##  So, I have had a rather large shift in direction with my build system.  I have started to use tup to 
##  execute the core of my build, and then use make to do more scripting things.  The reason for the shift 
##  is simple: tup is FAR easier to maintain than makefiles.  One of the key reasons for this is that the 
##  Tupfile is located in the target directory.  Therefore, if there is something that is needed to satisfy
##  a dependency, tup only needs to look in the directory where that dependency should be and if it is not 
##  there, read the Tupfile on how to create that object.
##
##  However, there is also give and take with tup.  Since I do not want to clutter up my sysroot folders with
##  a bunch of Tupfiles that would end up on the .iso or .img, I have implemnted the parts that would copy
##  these files into the sysroot folders in this makefile.  The result is MUCH simpler.  I have a default
##  'all' target that simply calls tup to refresh the build.  Any of the other specialized targets (.iso, 
##  or running QEMU) depend on the all target and then run the steps needed to complete the script.
##
##  The only key function I am giving up is the ability to build an architecture or a module independently.
##  For the moment, I think I can live with that by creating stub functions when needed.
##
## ----------------------------------------------------------------------------------------------------------------- 
##                                                                                                                 
##     Date     Tracker  Version  Pgmr  Description                                                                         
##  ----------  -------  -------  ----  ---------------------------------------------------------------------------
##  2017-03-26  Initial   0.0.0   ADCL  Initial version 
##  2017-05-10            0.0.0   ADCL  Gut this file in favor of tup and some short scripts
## 
#####################################################################################################################

.PHONY: all all-iso i686-iso run-i686 debug-i686 x86_64-iso run-x86_64 debug-x86_64 rpi2b-iso run-rpi2b debug-rpi2b
all:
	tup

all-iso: all i686-iso x86_64-iso rpi2b-iso

i686-iso: all
	rm -fR iso/i686.iso
	rm -fR sysroot/i686/*
	cp -fR bin/i686/* sysroot/i686/
	find sysroot/i686 -type f -name Tupfile -delete
	grub2-mkrescue -o iso/i686.iso sysroot/i686


run-i686: i686-iso
	qemu-system-i386 -m 3584 -serial stdio -cdrom iso/i686.iso


debug-i686: i686-iso
	qemu-system-i386 -m 3584 -serial stdio -cdrom iso/i686.iso -s -S


x86_64-iso: all
	rm -fR iso/x86_64.iso
	rm -fR sysroot/x86_64/*
	cp -fR bin/x86_64/* sysroot/x86_64/
	find sysroot/x86_64 -type f -name Tupfile -delete
	grub2-mkrescue -o iso/x86_64.iso sysroot/x86_64


run-x86_64: x86_64-iso
	qemu-system-x86_64 -m 8192 -serial stdio -cdrom iso/x86_64.iso


debug-x86_64: x86_64-iso
	qemu-system-x86_64 -m 8192 -serial stdio -cdrom iso/x86_64.iso -s -S


rpi2b-iso: all
	rm -fR iso/rpi2b.img
	cp -fR bin/rpi2b/* sysroot/rpi2b/
	find sysroot/rpi2b -type f -name Tupfile -delete
	mkdir -p ./p1
	dd if=/dev/zero of=iso/rpi2b.img count=20 bs=1M
	parted --script iso/rpi2b.img mklabel msdos mkpart p ext2 1 20 set 1 boot on
	sudo kpartx -as iso/rpi2b.img || true
	sudo mkfs.ext2 /dev/mapper/loop0p1
	sudo mount /dev/mapper/loop0p1 ./p1
	sudo cp -R sysroot/rpi2b/* p1/
	sudo umount ./p1
	sudo kpartx -d iso/rpi2b.img
	rm -fR ./p1


run-rpi2b: rpi2b-iso
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel ~/bin/kernel-qemu.img --hda iso/rpi2b.img


debug-rpi2b: rpi2b-iso
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel ~/bin/kernel-qemu.img --hda iso/rpi2b.img -s -S
