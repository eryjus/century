***Century OS***

Century is a Hobby Operating System.

        Copyright (c)  2017 -- Adam Clark (see LICENSE for the specifics)


I chose "Century" as a name because it described me and my project in 2 ways:
1) I am a road bicyclist.  In the US, the bicycling version of a marathon is called a 
   century -- 100 miles at a time.  No, I have no aspirations to ride in the TDF.
2) I expect this OS to take at least 100 years to complete.

Since this OS will be available for others to review and possibly glean nuggets of 
information.  I do not represent my code as the ideal solution for anything.  If you 
have feedback for me, please feel free to e-mail me at hobbyos@eryjus.com.  Take a look
at [my journal](./JOURNAL.md) if you haven't already.

As a result putting my own development stages in display in a public repository, I plan
to provide smaller commits to my public repository.  The hope behind the large number 
of smaller commits is that other readers can observe the evolution of development.  
OS development is a large project and many do not know what order to take tasks in.  
The public repository documents my own development path and changes in direction.


***Why?***

I admit, I have had several partial starts, some on my own and some with existing OSes.
So, why this OS and why now?  That's a fair question.

First, I am by no means an expert.  Each system I have started has had specific
objectives in mind.  My first incarnation was a basic learning project.  It lacked
focus and direction.  However, I was able to get multiple processes running properly.  
I also managed to break it and fix it several times.  It was 32-bit.

Then, I embarked on a 64-bit assembly system.  I took it on in assembly because I was
not able to figure out how to co-mingle 32-bit and 64-bit code in C.  Well, if you can't
fix it, feature it!  So, I took it on in 100% assembly.  Again, I was able to get multiple
processes running.

Then I looked at xv6, a teaching kernel which I refer back to even today.  I took that
as a starting point since it had a working filesystem and user-mode applications.  I learn
better when I have a working system to look review and dig into.  I was going to slowly 
replace portions with things of my own invention.

Well, after several years away, I have decided to pick up the OS again.  I had acquired a
Raspberry Pi 2 that I wanted to start playing with and never did.  I got tired of it 
sitting on my desk staring me down.  So, I started over with this incarnation of Century.

The goals here are pretty simple:
1. Document the progress of this OS for those that learn through another's mistakes.  Use
   GitHub to keep track of my history.
2. Establish a common kernel base source that can be run on multiple architectures, with 
   architecture-specific code handled properly.
3. Write this kernel as a micro-kernel.
4. Write the GUI desktop manager myself, but port as many other components as possible.
   Therefore, take time to make the porting process as easy as possible.

But, again, I am no expert.  You might just want to follow this project just to watch the 
train wreck unfolding!  ;)


***Some thoughts***

I have taken the development tasks in a breadth-first approach, developing support for
multiple platforms for the most basic "Hello World" kernel.  This has forced me to think
long and hard about how I want to organize my build system.  Also, it is driving me to 
build a device abstraction layer right off the bat in the code.  My hope is that the 
deferred gratification will save me a ton of time later in development.


***The Build System***

`make` is out!  Well, mostly.

I have started to convert to `tup` for the bulk of the build system.

So, I have had a rather large shift in direction with my build system.  I have started to use 
`tup` to execute the core of my build, and then use make to do more scripting things.  The 
reason for the shift is simple: `tup` is FAR easier to maintain than make files.  One of the key 
reasons for this is that the Tupfile is located in the target directory (where makefiles are 
typically located in the source directories).  Therefore, if there is something that is needed 
to satisfy a dependency, `tup` only needs to look in the directory where that dependency should 
be and if it is not there, read the Tupfile in that directory on how to create that object.

However, there is also give and take with `tup`.  Since I do not want to clutter up my sysroot 
folders with a bunch of Tupfiles that would end up on the .iso or .img, I have implemented the 
parts that would copy these files into the sysroot folders in the only Makefile.  The result is 
MUCH simpler.  I have a default 'all' target that simply calls `tup` to refresh the build.  Any of 
the other specialized targets (.iso, or running QEMU) depend on the 'all' target and then run the 
steps needed to complete the script.

The only key function I am giving up is the ability to build an architecture or a module 
independently.  For the moment, I think I can live with that by creating stub functions when needed.

You can find `tup` at: http://gittup.org/gittup/.


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
on a vmWare ESXi server connected iSCSI to a drobo b800i SAN) as my development PC.


***VMM Address Space***

As I begin to write my VMM, it helps to have a plan.  Some of this plan is taken from my 
recollection of other iterations of Century.  However, we have both 32-bit and 64-bit versions 
of the Century OS, and x86-family and ARM-family.  The memory maps are simply going to be 
different.

**i686**

I am taking inspiration from my conversation with Brendan, here: 
http://forum.osdev.org/viewtopic.php?f=1&t=28573, meaning the best thing to do in the x86_64 
space is to keep the kernel/fixed data in the last 2GB virtual address space.

So for 32-bit address space, we have a 2-level MMU table (each 4096 bytes in total): the Page 
Directory and the Page Table.  Each entry is 4 bytes long, resulting in 1024 entries in each
table.  Now, each entry points to a 4K page so that each Page Directory Entry (PDE) is
responsible for 1024 * 4096 bytes, or 4MB.  Therefore, it makes sense to break the 32-bit 
i686 architecture into 4MB chunks for mapping:

| Address Usage     | * | i686 Range Start | i686 Range End  | Size  | PDE Range   |
|-------------------|---|------------------|-----------------|-------|-------------|
| Recursive Mapping |   | `0xffc0 0000`    | `0xffff ffff`   |   4MB |    1023     |
| Kernel Stacks     | S | `0xff40 0000`    | `0xffbf ffff`   |   8MB | 1021 - 1022 | 
| Temporary Mapping | T | `0xff00 0000`    | `0xff3f ffff`   |   4MB |    1020     |
| Poison            | P | `0xf000 0000`    | `0xfeff ffff`   | 240MB |  960 - 1019 |
| Frame Buffer      | F | `0xe000 0000`    | `0xefff ffff`   | 256MB |  896 - 959  |
|   ** Unused **    |   | `0xc000 0000`    | `0xdfff ffff`   | 512MB |  768 - 895  |
| Slab Space        | L | `0xa000 0000`    | `0xbfff ffff`   | 512MB |  640 - 767  |
| Kernel Space      | K | `0x8000 0000`    | `0x9fff ffff`   | 512MB |  512 - 639  |
| User/Driver Space |   | `0x0000 0000`    | `0x7fff ffff`   |   2GB |    0 - 511  |

Notes:
S) Room enough for 512 X 16K stacks

T) We need a place to put frames for clearing or initialization.  This space allows for this 
activity.  All pages will be in this space temporarily.

P) Linux uses certain addresses to indicate NULL or uninitialized pointers (not 0 since that
is reserved for user space).  The benefit is that a page fault can easily reasonably identify 
what kind of object was incorrectly de-referenced.  The 2 drawbacks are that a simple comparison
to 0 is not going to work and the compiler will not complain about uninitialized variables.

F) This frame buffer is sized adequately size for 7680 X 4320 8K UHD (32-bit color depth).  
There is enough room allocated for 2 frames of this size.

L) The initial structure to communicate between the loader and the kernel will be placed here.

K) The kernel space is specifically for the typical executable (code, data, heap).
