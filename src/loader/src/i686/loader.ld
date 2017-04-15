/*******************************************************************************************************************/
/*                                                                                                                 */ 
/*  src/loader/src/i686/loader.ld -- This is the linker script for locating the sections in the target binary      */
/*                                                                                                                 */ 
/*        Copyright (c)  2017 -- Adam Clark                                                                        */
/*                                                                                                                 */ 
/*        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):                                             */
/*        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice                             */
/*        you can do whatever you want with this stuff. If we meet some day, and you                               */
/*        think this stuff is worth it, you can buy me a beer in return.                                           */
/*                                                                                                                 */ 
/* TODO: make this loader relocated into upper memory when the MMU has been properly setup                         */
/*                                                                                                                 */ 
/* --------------------------------------------------------------------------------------------------------------- */
/*                                                                                                                 */ 
/*     Date     Tracker  Version  Pgmr  Description                                                                */       
/*  ----------  -------  -------  ----  -------------------------------------------------------------------------  */
/*  2017-04-14  Initial   0.0.0   ADCL  Initial version                                                            */
/*                                                                                                                 */ 
/*******************************************************************************************************************/

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS {
    /*
     * -- Set the location where the binary will be loaded
     *    ------------------------------------------------
     */
     
    . = 0x100000; 
    _loaderStart = .;
    _textStart = .;
    
    /*
     * -- This is where the code will be placed.  Note that we put the init section right up front.  This
     *    is where we will gain control.
     *    -----------------------------------------------------------------------------------------------
     */
    .text : {
        *(.mboot)
        *(.init)
        *(.text)
    }
    . = ALIGN(4096);
    _textEnd = .;
    
    _roStart = .;
    /*
     * -- We drop in the read only data here
     *    ----------------------------------
     */
    .rodata : {
        *(.rodata)
    }
    . = ALIGN(4096);
    _roEnd = .;

    /* 
     * -- Include debugging information in loader memory 
     *    ----------------------------------------------
     */
    _stabStart = .;
	.stab : {
		*(.stab);
		*(.stabstr);
		BYTE(0)		/* Force the linker to allocate space for this section */
	}
    . = ALIGN(4096);
	_stabEnd = .;

    _dataStart = .;
    /*
     * -- We drop in the read/write data here
     *    -----------------------------------
     */
    .data : {
        *(.data)
    }
    . = ALIGN(4096);
    _dataEnd = .;

    _bssStart = .;
    /*
     * -- We drop in the 0-init read/write data here
     *    ------------------------------------------
     */
    .bss : {
        *(.bss)
    }
    . = ALIGN(4096);
    _bssEnd = .;
    _loaderEnd = .;
}