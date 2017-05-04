//===================================================================================================================
//
//  loader i686/hw.h -- Hardware locations
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
//  2017-04-17  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __HW_H_INCLUDED__
#define __HW_H_INCLUDED__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


//
// -- The BIOS Data Area identified system capabilities
//    -------------------------------------------------
#define BDA             0x400

#define BDA_COM1        0x00
#define BDA_COM2        0x02
#define BDA_COM3        0x04
#define BDA_COM4        0x06
#define BDA_LPT1        0x08
#define BDA_LPT2        0x0a
#define BDA_LPT3        0x0c
#define BDA_EBDA        0x0e
#define BDA_VIDEO       0x63


//
// -- Serial Ports
//    ------------
#define COM1                0x3f8

#define UART_RBR            0x00                    // Receiver Buffer Register (read only)
#define UART_THR            0x00                    // Transmitter Holding Register (write only)
#define UART_IER            0x01                    // Interrupt Enable Register
//-------------------------------------------------------------------------------------------------------------------
#define UART_ERBFI          (1<<0)                  // Enable Received Data Available Interrupt
#define UART_ETBEI          (1<<1)                  // Enable Transmitter Holding Register Empty Interrupt
#define UART_ELSI           (1<<2)                  // Enable Receiver Line Status Interrupt
#define UART_EDSSI          (1<<3)                  // Enable MODEM Status Interrupt

#define UART_IIR            0x02                    // Interrupt Identification Register (read only)
//-------------------------------------------------------------------------------------------------------------------
#define UART_PEND           (1<<0)                  // '0' if interrupt pending
#define UART_INTID          (7<<1)                  // Interrupt Identification
#define UARTIID_RLS         (0b011<<1)              // Receiver Line Status
#define UARTIID_RDA         (0b010<<1)              // Received Data Available
#define UARTIID_CTO         (0b110<<1)              // Character Timeout
#define UARTIID_THRE        (0b001<<1)              // Transmitter Holding Register Empty
#define UARTIID_MODEM       (0b000<<1)              // Modem Status
#define UARTIID_FIFO        (3<<6)                  // FIFOs enabled (always 0b11 when FIFO 1)

#define UART_FCR            0x02
//-------------------------------------------------------------------------------------------------------------------
#define UART_FIFO           (1<<0)                  // FIFOe Enabled
#define UART_RCVRFIFO       (1<<1)                  // RCVR FIFO Reset
#define UART_XMITFIFO       (1<<2)                  // XMIT FIFO Reset
#define UART_DMAMDOE        (1<<3)                  // DMA Mode Select
#define UART_RCVRTRGLSB     (1<<6)                  // RCVR Trigger LSB
#define UART_RCVRTRGMSB     (1<<7)                  // RCVR Trigger MSB

#define UART_LCR            0x03
//-------------------------------------------------------------------------------------------------------------------
#define UART_WLS            (3<<0)                  // Word Length Select
#define UART_WLS5           (0b00<<0)               // 5-bit words
#define UART_WLS6           (0b01<<0)               // 6-bit words
#define UART_WLS7           (0b10<<0)               // 7-bit words
#define UART_WLS8           (0b11<<0)               // 8-bit words
#define UART_STP            (1<<2)                  // 1.5 or 2 Stop Bits
#define UART_PEN            (1<<3)                  // Parity Enable
#define UART_EPS            (1<<4)                  // Even Parity Select
#define UART_STICK          (1<<5)                  // Stick Parity
#define UART_BREAK          (1<<6)                  // Set Break
#define UART_DLAB           (1<<7)                  // Divisor Latch Access Bit

#define UART_MCR            0x04
//-------------------------------------------------------------------------------------------------------------------
#define UART_DTR            (1<<0)                  // Data Terminal Ready
#define UART_RTS            (1<<1)                  // Request to Send
#define UART_OUT1           (1<<2)                  // Out 1
#define UART_OUT2           (1<<3)                  // Out 2
#define UART_LOOP           (1<<4)                  // loop

#define UART_LSR            0x05
//-------------------------------------------------------------------------------------------------------------------
#define UART_DR             (1<<0)                  // Data Ready
#define UART_OE             (1<<1)                  // Overrun Error
#define UART_PE             (1<<2)                  // Parity Error
#define UART_FE             (1<<3)                  // Framing Error
#define UART_BI             (1<<4)                  // Break Interrupt
#define UART_THRE           (1<<5)                  // Transmitter Holding Register Empty
#define UART_TEMT           (1<<6)                  // Transmitter Empty
#define UART_RFE            (1<<7)                  // Error in receiver FIFO

#define UART_MSR            0x06
//-------------------------------------------------------------------------------------------------------------------
#define UART_DCTS           (1<<0)                  // Delta Clear to Send
#define UART_DDSR           (1<<1)                  // Delta Data Set Ready
#define UART_TERI           (1<<2)                  // Trailing Edge Ring Indicator
#define UART_DDCD           (1<<3)                  // Delta Data Carrier Detect
#define UART_CTS            (1<<4)                  // Clear to Sent
#define UART_DSR            (1<<5)                  // Data Set Ready
#define UART_RI             (1<<6)                  // Ring Indicator
#define UART_DCD            (1<<7)                  // Data Carrier Detected

#define UART_SCR            0x07
#define UART_DLL            0x00
#define UART_DLM            0x01



#endif
