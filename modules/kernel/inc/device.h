//===================================================================================================================
//
//  kernel device.h -- This file contains the common interfaces to represent all devices in a system
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
// Some thoughts about how I am organizing the devices and drivers.  First, I am taking a bottom-up approach to 
// development, with each layer above the pervious one providing additional abstractions and control/checking.
//
// The device level is strictly responsible for communicating with the hardware.  Very little checking will be 
// some at this level and therefore, it will be possible for mismanaged drivers to all send commands to a single
// device that is owned multiple times.  At the device level, the only sanity check will be about existence.
// Devices will likely need to be coded unique across platforms.
//
// A driver will then abstract and manage that device for the OS.  There is a high likelihood of having more than
// one device for any given driver (say serial ports).  This layer will be responsible for making sure that 
// multiple processes are not using the same device when not allowed.  This layer will be closely aligned to how
// the device natively handles the data (say, block vs character).  Further abstractions above this (file-systems
// and files as an example) will be built on top of this layer as appropriate.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-08  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __DEVICE_H_INCLUDED__
#define __DEVICE_H_INCLUDED__


#include <stdbool.h>


//-------------------------------------------------------------------------------------------------------------------
// These are the different classes of devices we are establishing for the os
//-------------------------------------------------------------------------------------------------------------------
typedef enum {
    DEV_DEVICE,
    DEV_SERIAL,
} device_class;


//-------------------------------------------------------------------------------------------------------------------
// At the lowest level, all devices on the system are represented as a device structure.  All device structs will
// have this structure as its first member and can be statically converted to that structure based on its type.
// --------------------------------------------------------------------------------------------------------------
// .parent          -- the parent device in a device tree -- typically a bus or bridge
// .deviceName      -- This is the name of the device (might be found in the /dev file system eventually)
// .devClass        -- This is the class of device, which will indicate that comes behind these fields
// .valid           -- Some default devices may or may not exist on the system; this indicates if this driver can 
//                     be used
//-------------------------------------------------------------------------------------------------------------------
struct device {
    struct device *parent;
    char *devName;
    device_class devClass;
    bool valid;
};


//
// -- include all the different classes of devices supported
//    ------------------------------------------------------
#include "serial.h"

#endif
