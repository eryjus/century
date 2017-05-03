***loader Module***

The loader module's purpose is the perform the early initialization of the system and 
transfer control to the kernel.  This module will receive control from the multiboot
second stage and will perform the early initialization to put the system into its 
native mode.  The following will be completed by the multiboot loader:

* The screen will be placed into graphics mode.  This mode can be consistent across 
  all architectures and allow the actual OS driver take over the video responsibility.
  The minimum supported video resolution will be 1024 X 768 X 16.

* The MMU will be initialized for the architecture.  

* The loader will initialize the UART and have that available for optionally dumping 
  debug information to that port.
  
The loader is inspired by the following wiki page (or at least as it was on 13-Apr-2017):
http://wiki.osdev.org/Creating_a_64-bit_kernel_using_a_separate_loader


**rpi2b Architecture**

There is an open source multiboot project called rpi-boot.  This project reports to 
supply a multiboot-compliant second stage for ARM (and specifically for the RPi).  
Now, this does not specifically mention RPi2 (which has a different hardware address
space), but I'm willing to bet I can get it working.  The project is still active
as of Jan-2017.  This project is at https://github.com/jncronin/rpi-boot


**Unicode Support**

Century will support Unicode.  It's silly not to.  However, the question is: to what
level?  Here are some thoughts:

* UTF-8 is the default encoding.
* Certain portions (such as the loader) will only support code points 0 through 127
  (ASCII).  This restriction is reasonable since the font used would be too big 
  with support more than this.  For any code point >127, the code point will be
  ignored (not printed).  A '?' will be printed in place.
* When printing/displaying a code point, all representations will be converted into 
  UTF-32 encoding.
