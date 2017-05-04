//===================================================================================================================
//
//  kernel i686/hw.h -- This file aggregates all the hardware port definitions and bit flags.
//
//        Copyright (c)  2017 -- Adam Clark
//
//        "THE BEER-WARE LICENSE" (by Poul-Henning Kamp; Revision 42):
//        <hobbyos@eryjus.com> wrote this software.  As long as you retain this notice
//        you can do whatever you want with this stuff. If we meet some day, and you
//        think this stuff is worth it, you can buy me a beer in return.
//
// I will be taking a slightly different approach with the i686 version of this file...  I will be developing it
// as I go.  The main reason for this that the x86 version does not have a single config that is consistent across
// all hardware like the RPi does.  I will likely have to parse the RSDT to find the information I need at runtime.
//
// -----------------------------------------------------------------------------------------------------------------
//
//     Date     Tracker  Version  Pgmr  Description
//  ----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2017-03-28  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================

#ifndef __HW_H_INCLUDED__
#define __HW_H_INCLUDED__

//-------------------------------------------------------------------------------------------------------------------
// Com1, Com2, Com3, Com4
//-------------------------------------------------------------------------------------------------------------------


//
// -- The base ports for all typical com ports
//    ----------------------------------------
#define COM1                (0x3f8)
#define COM2                (0x2f8)
#define COM3                (0x3e8)
#define COM4                (0x2e8)

//
// -- Port offsets
//    ------------
#define COM_RBR             (0)                         // Receive Buffer Reg (read only)
//-------------------------------------------------------------------------------------------------------------------
#define COMRBR              (0xff)                      // The data size


#define COM_THR             (0)                         // Transmitter Holding Reg (write only)
//-------------------------------------------------------------------------------------------------------------------
#define COMTHR              (0xff)                      // The data size


#define COM_IER             (1)                         // Interrupt Enable Register
//-------------------------------------------------------------------------------------------------------------------
#define COMIER_EDSSI        (1<<3)                      // Enable Delta Status Signals Interrupt
#define COMIER_ELSI         (1<<2)                      // Enable Line Status Interrupt
#define COMIER_ETBEI        (1<<1)                      // Enable Transmitter Buffer Empty Interrupt
#define COMIER_ERBFI        (1<<0)                      // Enable Receiver Buffer Full Interrupt


#define COM_DLLSB           (0)                         // DLAB=1, Divisor Latch LSB
//-------------------------------------------------------------------------------------------------------------------
#define COMDLLSB            (0xff)                      // The LSB of the Baud Divisor


#define COM_DLMSB           (1)                         // DLAB=1, Divisor Latch MSB
//-------------------------------------------------------------------------------------------------------------------
#define COMDLMSB            (0xff)                      // The MSB of the Baud Divisor


#define COM_IIR             (2)                         // Interrupt Identification (read only)
//-------------------------------------------------------------------------------------------------------------------
#define COMIIR_FIFOTX       (1<<7)                      // Transmit FIFO enable  LEARN: Is this really TX? or RX?
#define COMIIR_FIFORX       (1<<6)                      // Receive FIFO enable   LEARN: Is this really RX? or TX?
#define COMIIR_IID2         (1<<3)                      // Interrupt ID #2
#define COMIIR_IID1         (1<<2)                      // Interrupt ID #1
#define COMIIR_IID0         (1<<1)                      // Interrupt ID #0
#define COMIIR_PND          (1<<0)                      // Interrupt Pending


#define COM_FCR             (2)                         // FIFO control (write only)
//-------------------------------------------------------------------------------------------------------------------
#define COMFCR_RXTRG        (3<<6)                      // Trigger Level of the RX FIFO
#define COMFCR_DMA          (1<<3)                      // DMA Mode
#define COMFCR_CLRTX        (1<<2)                      // Clear TX FIFO
#define COMFCR_CLRRX        (1<<1)                      // Clear RX FIFO
#define COMFCR_FIFOEN       (1<<0)                      // FIFO Enabled

#define SH_COMFCRRXTRG1     (0b00<<6)                   // Shift this value to the right bits
#define SH_COMFCRRXTRG4     (0b01<<6)                   // Shift this value to the right bits
#define SH_COMFCRRXTRG8     (0b10<<6)                   // Shift this value to the right bits
#define SH_COMFCRRXTRG14    (0b11<<6)                   // Shift this value to the right bits


#define COM_LCR             (3)                         // Line Control
//-------------------------------------------------------------------------------------------------------------------
#define COMLCR_DLAB         (1<<7)                      // DLAB Enable
#define COMLCR_SBR          (1<<6)                      // Set Break
#define COMLCR_STICKPAR     (1<<5)                      // Stick Parity Select
#define COMLCR_EVENPAR      (1<<4)                      // Even Parity Select
#define COMLCR_PAREN        (1<<3)                      // Parity Enable
#define COMLCR_STOP         (1<<2)                      // Stop Bits
#define COMLCR_WORD         (3<<0)                      // Word Length

#define SH_COMLCRWORD_8     ((0b11)<<0)                 // 8 bit word length
#define SH_COMLCRWORD_7     ((0b10)<<0)                 // 7 bit word length
#define SH_COMLCRWORD_6     ((0b01)<<0)                 // 6 bit word length
#define SH_COMLCRWORD_5     ((0b00)<<0)                 // 5 bit word length



#define COM_MCR             (4)                         // Modem Control
//-------------------------------------------------------------------------------------------------------------------
#define COMMCR_LOOP         (1<<4)                      // Local Loopback
#define COMMCR_OUT2         (1<<3)                      // Interrupts sent to ICU
#define COMMCR_OUT1         (1<<2)                      // ????
#define COMMCR_RTS          (1<<1)                      // RTS
#define COMMCR_DTR          (1<<0)                      // DTR


#define COM_LSR             (5)                         // Line Status
//-------------------------------------------------------------------------------------------------------------------
#define COMLSR_FIFOERR      (1<<7)                      // FIFO Error
#define COMLSR_TEMT         (1<<6)                      // Tx Empty
#define COMLSR_THRE         (1<<5)                      // Tx Holding Register Empty (data can be written)
#define COMLSR_BREAK        (1<<4)                      // Broken Line
#define COMLSR_FE           (1<<3)                      // Framing Error
#define COMLSR_PE           (1<<2)                      // Parity Error
#define COMLSR_OE           (1<<1)                      // Overrun Error
#define COMLSR_RBF          (1<<0)                      // Receive Buffer Full


#define COM_MSR             (6)                         // Modem Status
//-------------------------------------------------------------------------------------------------------------------
#define COMMSR_DCD          (1<<7)                      // Data Carrier Detect
#define COMMSR_RI           (1<<6)                      // Ring Indicator
#define COMMSR_DSR          (1<<5)                      // Data Set Ready
#define COMMSR_CTS          (1<<4)                      // Clear to Send
#define COMMSR_DDCD         (1<<3)                      // Delta Data Carrier Detect
#define COMMSR_TERI         (1<<2)                      // Trailing Edge Ring Indicator
#define COMMSR_DDSR         (1<<1)                      // Delta Data Set Ready
#define COMMSR_DCTS         (1<<0)                      // Delta Clear To Send


#define COM_SCR             (7)                         // Scratch Register
//-------------------------------------------------------------------------------------------------------------------
// -- This is unused

#endif
