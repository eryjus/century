***Prologue***

So as I am sitting down today to start laying out how my virtual memory manager will be used, 
I really should be documenting my progress and thoughts if this is really going to be 
something others can learn from.  The README.md file is not really suited for the type
of "conversation" I want to have here.  One of the goals of this project is, after all,
to have a result that others can learn from -- even if it is from my mistakes.

So, what has happened to this point that I really should take a moment to catch up on
before I get into the rest of the decision making.  Well....

I have decided on the initial architectures I would like to support:  i686, x86_64, and
Raspberry Pi 2b (or 'rpi2b' as you will come to recognize throughout).  I also have an
rpi3b sitting on my desk, but the ARM architecture if foreign enough at this point I want
to make sure I can get through that architecture before I take on a new one.

I have been able to set up an emulator for each of the architectures and I am able to use
make to invoke each architecture to test.  My memory is not as sharp as it used to be and
so I cannot always recall the exact command I want to type for each emulator -- there are 
slight differences.

I have built up a massive (and admittedly difficult to maintain) make system, and have torn
the core of that make system down in favor of tup.  I did talk about it a bit on osdev.org, 
here: http://forum.osdev.org/viewtopic.php?f=13&p=275461.

I also have taken on the rpi-boot second stage to boot the rpi2b architecture (compare that
to GRUB for x86-family).  I have a loader (which has much my focus to-date), and a kernel 
(which is loaded my the second stage but never gets control, yet).

And I have a bitmap physical memory allocator (at least initialization) started.  Everything
boots and the kernel module is loading.

I did start to parse the ACPI tables, and then decided to rip that code back out.  This 
deserves a bit of conversation.  I have conflicting goals and I had to figure out how to 
reconcile them.  The conflicts were: 1) all 1-time initialization was going to be performed
in the loader; and 2) the CPU was going to be placed into its native mode when jumping
from the loader to the kernel.  Well, the issue was that the ACPI tables were not all 
guaranteed to be locatable by 32-bit code.  This means the for 64-bit systems, I would need
to either read the table in the kernel module, or would need to get into the native CPU mode
prior to the jump to the kernel code in order to complete the initialization.  I chose to 
delay this decision by postponing the integration of the ACPI tables.

So, my priority is to get into the native CPU mode and then transfer control to the kernel
module.  This change of course makes my Virtual Memory Manager (VMM) the next on the list.

I wanted to incorporate all the thinking I had done with Century64 (the assembly-based 64-bit
version), but I just realized that the documentation was lost in the "Big SAN Crash of 2015!"

---

***2017-May-14***

I started to lay out the virtual memory today.  I intended to lay out the memory for all the
architectures but quickly realized the differences that are required.  So, I stepped back to
start with the 32-bit x86-family.  I stated a few of the base functions that will be used
to manage this memory.

At the same time, I had to add some additional functions for the Physical Memory Manager that
I was not expecting to need (check an allocation and allocate a new frame).

Also, the things to do list is getting to be a bit more than I can keep track of.  I need 
a separate tool to keep track of them, but I do not want to include them all in the code
as TODO: items and I do not want to create a GitHub bug for them.  I have a Redmine server
as a part of my home computer environment, so I am going to use that.  Forgive me for not
having a public to-do list. 

---

***2017-May-15***

So, the first thing to do today was to implement the missing function FrameNew(), which is
exceptionally trivial.  I will not be allocating too many frames in the loader and have no
desire to reuse this code in the kernel.  The algorithm used starts from the high address 
point and walks back down toward 0.  This is so that the frames we allocate here for the 
MMU structures are generally out of the way and not going to be used soon (and probably 
not ever released).

At the same time, it took on the task to properly set the function prototypes in proto.h.
This change enabled the elimination of the phys-mm.h file.

After that I spent the rest of the evening sorting out the physical memory manager and MMU
initialization.  As I call it a night, I think I have it all working.  I will test again 
tomorrow.

---

***2017-May-16***

Today, as I was considering the additional MMU mappings I needed, the frame buffer got in the 
way....  I have to figure out where I want to put this in the overall memory map.  This led 
me to scrap completely the map I had started with and develop a new one.  Of particular note
is the plan to adopt Linux's use of a "poison" memory space.  This space will generate a 
page fault with any reference that theoretically will lead to the *type* of object improperly
initialized.  More information is available in the linux source: 
http://elixir.free-electrons.com/linux/latest/source/include/linux/poison.h.

Now, when I go back to fix up the phys-mm.c initialization, I realize that there is a lot of
instances where I am looping over a number of frames to allocate or free.  It becomes a good 
opportunity for code cleaning.  So, I'm taking this on.

At the end of the day, I have been able to map the target frame buffer, but I am still having 
issues with the phys-mm.c initialization.  In particular, MB2 structures appear to be 
reporting all memory ranges as available.  I may have to go back to the MB2 parser to address 
this.

---

***2017-May-17***

What a difference a day makes...  I was able to pinpoint the MB2 issue within seconds with a 
fresh perspective.  It always amazes me how walking away from a problem can actually help you
solve it faster.  So, at this point, both the MB1 and MB2 boots of i686 match.  Good!

The MMU is not complete yet, but I will back out all my debugging code and commit an 
intermediate checkpoint.  This is a good point to be able to roll back to if need be.