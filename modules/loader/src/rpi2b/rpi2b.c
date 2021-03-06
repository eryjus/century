//===================================================================================================================
//
//  loader rpi2b/rpi2b.c -- High-level functions specific to the rpi2b arcitecture
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-19  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


//-------------------------------------------------------------------------------------------------------------------
// GetArch() -- Get a short string to identify the architecture
//-------------------------------------------------------------------------------------------------------------------
const char *GetArch(void)
{
    return u8"rpi2b";
}
