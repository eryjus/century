***Century OS***

Century is a Hobby Operating System.

        Copyright (c)  2017 -- Adam Clark (see LICENSE for the specifics)


I chose "Century" as a name because it described me and my project in 2 ways:
1) I am a road bicyclist.  In the US, the bicycling version of a marathon is called a 
   century -- 100 miles at a time.  No, I have no aspirations to ride in the TDF.
2) I expect this OS to take at least 100 years to complete.

Since this OS will be available for others to review and possibly glean nuggets of 
information.  I do not represent my code as the ideal solution for anything.  If you 
have feedback for me, please feel free to e-mail me at hobbyos@eryjus.com.

As a result putting my own development stages in display in a public repository, I plan
to provide smaller commits to my public repository.  The hope behind the large number 
of smaller commits is that other readers can observe the evolution of development.  
OS development is a large project and many do not know what order to take tasks in.  
The public repository documents my own development path and changes in direction.


***Some thoughts***

I have taken the development tasks in a breadth-first approach, developing support for
multiple platforms for the most basic "Hello World" kernel.  This has forced me to think
long and hard about how I want to organize my build system.  Also, it is driving me to 
build a device abstraction layer right off the bat in the code.  My hope is that the 
deferred gratification will save me a ton of time later in development.


***The Build System***

I have a very complicated build system.  I know this; I accept this.  

I absolutely hate having to maintain hard targets for each of my build and would rather
spend extra time (OK, a _LOT_ of extra time) setting up my build system to meet my needs.
I figure this way: a significant part of my development workflow is to type the command
`make <target>` frequently.  When I do this, I want a few things to take place:
1. The target gets built taking into account all the new dependencies.
2. Anything that needs to be rebuilt gets rebuilt; things that do not need to be rebuilt
   does not get rebuilt.
3. I want it to be quick, given the number of times I will do this.

The thing that breaks my list above is changing the Makefile/Makefrag.mk files.  As a 
result, I do not want to have to make changes in these files nowhere near as frequently 
as I type the command `make`.  To meet my goals, my build system ends up being far more 
complicated than most others will be.

See the Makefile header comments for some popular `make` targets.


***The Cross-Compiler***

For the record, this OS is being compiled with the following software versions:
* binutils-2.28
* gcc-6.3.0
I will likely stick with what works and not update is at all, unless I find a compelling 
reason to rebuild all my tools.

You will need to have cross compiled both binutils and gcc for the following targets, using 
the instructions found here: http://wiki.osdev.org/GCC_Cross-Compiler
* arm-eabi-
* i686-elf-
* x86_64-elf-

After building and installing each binutils / gcc suite of tools, I recommend rebooting.

Finally, rpi-boot (a Raspberry Pi Multiboot second stage loader) will need to be compiled
using the arm-eabi- tool set.  Note that rpi-boot expects the tools to be named 
arm-none-eabi-, and I built mine as arm-eabi-.  You should be able to reconcile the 
discrepancies by either changing the rpi-boot makefile or building another cross-compiler.

rpi-boot can be found here: https://github.com/jncronin/rpi-boot.

Other than those pieces of software and their dependencies, I am really not using anything
extraordinary in my build yet.  Depending on your distro, you might need `kpartx` and 
`grub2-mkrescue` tools.  Also, you will want to review the kernel/rpi2b/Makefrag.mk header
comments as it lays out some specific requirements for `sudo` to make the build easier.
I use qemu as my emulator as it supports all 3 of my target architectures.

Also, in case anyone is curious, I am using Visual Studio Code for my IDE and FC25 (running 
on a vmWare ESXi server connected iSCSI to a drobo 800i SAN) as my development PC.
