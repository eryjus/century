//===================================================================================================================
//
//  kernel serial.h -- This file contains the common interfaces for the serial ports
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
// The serial device is the first multi-hardware device structure I am developing.  As this is developed out, I 
// will be adding lots to this file; it will be quite dynamic for a while.  At the moment, I am only considering
// 32-bit address space (for MMIO) and hardware I/O ports.  64-bit address space will change this definition a 
// bit (likely with #defines embedded in the strucure though I think I would like to avoid that if possible).  So:
//
// TODO: with x86_84, update the hardware port address to allow for 64-bit address space.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-04-08  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __SERIAL_H_INCLUDED__
#define __SERIAL_H_INCLUDED__


#ifndef __DEVICE_H_INCLUDED__
#error "Make sure you are including device.h, which will include this file.  This file was processed before device.h"
#endif


//
// -- some forward declarations
//    -------------------------
struct serial_ops;


//-------------------------------------------------------------------------------------------------------------------
// This is the representation of a serial device
// ---------------------------------------------
// .dev             -- the implementation of the basic device structure, which all device structs include as the 
//                     first element
// .hwInterface     -- either the I/O port or the MMIO register, depending on the architecture
// .ops             -- the function list that can be executed on this device
//-------------------------------------------------------------------------------------------------------------------
struct serial_device {
    struct device dev;
    uint32_t hwInterface;
    const struct serial_ops *ops;
};


//-------------------------------------------------------------------------------------------------------------------
// These are the standard operations that can be done on a Serial Device
// ---------------------------------------------------------------------
// .init            -- initialize the driver; determine if it is valid; DO NOT CALL THIS TWICE
// .open            -- open the device with the attached configuration for use
// .close           -- release the serial port
// .chgConfig       -- change the serial port configuration (baud-parity-stop)
// .rxFull          -- is the receive FIFO full?
// .rxReady         -- is there data to read?
// .rxStart         -- enable receiving data
// .rxStop          -- disable receiving data
// .txEmpty         -- is the send FIFO full?
// .txRoom          -- is there room to write data?
// .txStart         -- enable sending data
// .txStop          -- disable sending data
//-------------------------------------------------------------------------------------------------------------------
struct serial_ops {
    void (*init)(struct serial_device *dev, uint32_t ifc);
    void (*open)(struct serial_device *dev, uint32_t baud, uint32_t bits, char parity, uint32_t stop);
    void (*close)(struct serial_device *dev);
    void (*chgConfig)(struct serial_device *dev, uint32_t baud, uint32_t bits, char parity, uint32_t stop);
    bool (*rxFull)(struct serial_device *dev);
    bool (*rxReady)(struct serial_device *dev);
    void (*rxStart)(struct serial_device *dev);
    void (*rxStop)(struct serial_device *dev);
    bool (*txEmpty)(struct serial_device *dev);
    bool (*txRoom)(struct serial_device *dev);
    void (*txStart)(struct serial_device *dev);
    void (*txStop)(struct serial_device *dev);
    uint8_t (*rx)(struct serial_device *dev);
    void (*tx)(struct serial_device *dev, uint8_t byte);
};



#endif
