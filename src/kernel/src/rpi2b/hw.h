//===================================================================================================================
//
//  kernel rpi2b/ports.h -- This file aggregates all the hardware port definitions and bit flags.
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
//  2017-03-28  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#define HW_BASE             (0x3f000000)                // this is the base address for all hardware I/O addresses


//-------------------------------------------------------------------------------------------------------------------
// Auxiliaries: UART1 & SPI1 & SPI2
//-------------------------------------------------------------------------------------------------------------------


//
// -- The 3 auxiliary peripherals (which are intermingled and controlled together)
//    ----------------------------------------------------------------------------

#define AUX_IRQ             (HW_BASE+0x215000)          // Auxiliary Interrupt Status
//-------------------------------------------------------------------------------------------------------------------
#define AUXIRQ_UART         (1<<0)                      // For AUX_IRQ, UART interrupt pending?
#define AUXIRQ_SPI1         (1<<1)                      // For AUX_IRQ, SPI1 interrupt pending?
#define AUXIRQ_SPI2         (1<<2)                      // For AUX_IRQ, SPI2 interrupt pending?


#define AUX_ENABLES         (HW_BASE+0x215004)          // Auxiliary Enables
//-------------------------------------------------------------------------------------------------------------------
#define AUXENB_UART         (1<<0)                      // For AUX_IRQ, UART enable
#define AUXENB_SPI1         (1<<1)                      // For AUX_IRQ, SPI1 enable
#define AUXENB_SPI2         (1<<2)                      // For AUX_IRQ, SPI2 enable


//
// -- The auxiliary Mini UART
//    -----------------------

#define AUX_MU_IO_REG       (HW_BASE+0x215040)          // Mini UART I/O Data
//-------------------------------------------------------------------------------------------------------------------
#define AUXMUART_TX         (0xff)                      // Transmit data bits
#define AUXMUART_RX         (0xff)                      // Receive data bits


#define AUX_MU_IER_REG      (HW_BASE+0x215044)          // Mini UART Interrupt Enable
//-------------------------------------------------------------------------------------------------------------------
#define AUXMUIER_RXENB      (1<<1)                      // Generate interrupt when RX FIFO queue has data
#define AUXMUIER_TXENB      (1<<1)                      // Generate interrupt when TX FIFO queue is empty


#define AUX_MU_IIR_REG      (HW_BASE+0x215048)          // Mini UART Interrupt Identify
//-------------------------------------------------------------------------------------------------------------------
#define AUXMUIIR_TXEMPTY    (1<<1)                      // Set when TX FIFO is empty
#define AUXMUIIR_RXEMPTY    (2<<1)                      // Set when RX FIFO is empty
#define AUXMUIIR_RXCLR      (1<<1)                      // Clear RX FIFO queue
#define AUXMUIIR_TXCLR      (2<<1)                      // Clear RX FIFO queue


#define AUX_MU_LCR_REG      (HW_BASE+0x21504c)          // Mini UART Line Control
//-------------------------------------------------------------------------------------------------------------------
#define AUXMULCR_DLAB       (1<<7)                      // Set to access baud rate register; clear for operation
#define AUXMULCR_BRK        (1<<6)                      // Set to indicate break conditions
#define AUXMULCR_SIZE       (1<<0)                      // Data Size: 0 = 7-bits; 1 = 8 bits


#define AUX_MU_MCR_REG      (HW_BASE+0x215050)          // Mini UART Modem Control
//-------------------------------------------------------------------------------------------------------------------
#define AUXMUMCR_RTS        (1<<1)                      // Clr if RTS it high; Set if RTS is low


#define AUX_MU_LSR_REG      (HW_BASE+0x215054)          // Mini UART Line Status
//-------------------------------------------------------------------------------------------------------------------
#define AUXMULSR_TXIDL      (1<<6)                      // Set if TX FIFO is empty and transmitter is idle
#define AUXMULSR_TXRMPTY    (1<<5)                      // Set if TX FIFO can accept at least 1 char
#define AUXMULSR_RXOVER     (1<<1)                      // Set if RX FIFO overrun (recent chars discarded)
#define AUXMULSR_RXRDY      (1<<0)                      // Set if RX FIFO has 1 char


#define AUX_MU_MSR_REG      (HW_BASE+0x215058)          // Mini UART Modem Status
//-------------------------------------------------------------------------------------------------------------------
#define AUXMUMSR_CTS        (1<<5)                      // Set if CTS is low; clr if CTS hi


#define AUX_MU_SCRATCH      (HW_BASE+0x21505c)          // Mini UART Scratch
//-------------------------------------------------------------------------------------------------------------------
#define AUXMU_BYTE          (0xff)                      // Single byte of storage


#define AUX_MU_CNTL_REG     (HW_BASE+0x215060)          // Mini UART Extra Control
//-------------------------------------------------------------------------------------------------------------------
#define AUXMUCTL_CTS        (1<<7)                      // clear if CTS assert level low
#define AUXMUCTL_RTS        (1<<6)                      // clear if RTS assert level low
#define AUXMUCTL_RTSAUTO    (3<<4)                      // de-assert: 00: 3 Byt; 01: 2 Byt; 10: 1 Byt; 11: 4 Byt
#define AUXMUCTL_ENBCTSAUTO (1<<3)                      // enable auto flow control using CTS
#define AUXMUCTL_ENBRTSAUTO (1<<2)                      // enable auto flow control using RTS
#define AUXMUCTL_ENBTX      (1<<1)                      // enable transmitter
#define AUCMUCTL_RNBRX      (1<<0)                      // enable receiver


#define AUX_MU_STAT_REG     (HW_BASE+0x215064)          // Mini UART Extra Status
//-------------------------------------------------------------------------------------------------------------------
#define AUXMUST_TXLVL       (0xf<<24)                   // TX FIFO level (0-8)
#define AUXMUST_RXLVL       (0xf<<16)                   // RX FIFO level (0-8)
#define AUXMUST_TXDONE      (1<<9)                      // Logical And of AUXMUST_TXEMPTY & AUXMUST_RXIDLE
#define AUXMUST_TXEMPTY     (1<<8)                      // TX FIFO empty (AUXMUST_TXLVL == 0)
#define AUXMUST_CTS         (1<<7)                      // Status of CTS line
#define AUXMUST_RTS         (1<<6)                      // Status of RTS line
#define AUXMUST_TXFULL      (1<<5)                      // TX FIFO Full
#define AUXMUST_RXOVER      (1<<4)                      // Receiver Overrun
#define AUXMUST_TXIDLE      (1<<3)                      // TX Idle (!AUXMUST_SPACE)
#define AUXMUST_RXIDLE      (1<<2)                      // RXC Idle
#define AUXMUST_SPACE       (1<<1)                      // TX FIFO has room
#define AUXMUST_AVAIL       (1<<0)                      // RX FIFO has data


#define AUX_MU_BAUD_REG     (HW_BASE+0x215068)          // Mini UART Baudrate
//-------------------------------------------------------------------------------------------------------------------
#define AUXMU_BAUD          (0xffff)                    // Baud Rate


//
// -- The auxiliary SPI 1
//    -------------------
#define AUX_SPI0_CNTL0_REG  (HW_BASE+0x215080)          // SPI 1 Control register 0
#define AUX_SPI0_CNTL1_REG  (HW_BASE+0x215084)          // SPI 1 Control register 1
#define AUX_SPI0_STAT_REG   (HW_BASE+0x215088)          // SPI 1 Status
#define AUX_SPI0_IO_REG     (HW_BASE+0x215090)          // SPI 1 Data
#define AUX_SPI0_PEEK_REG   (HW_BASE+0x215094)          // SPI 1 Peek


//
// -- The auxiliary SPI 2
//    -------------------
#define AUX_SPI1_CNTL0_REG  (HW_BASE+0x2150c0)          // SPI 2 Control register 0
#define AUX_SPI1_CNTL1_REG  (HW_BASE+0x2150c4)          // SPI 2 Control register 1
#define AUX_SPI1_STAT_REG   (HW_BASE+0x2150c8)          // SPI 2 Status
#define AUX_SPI1_IO_REG     (HW_BASE+0x2150d0)          // SPI 2 Data
#define AUX_SPI1_PEEK_REG   (HW_BASE+0x2150d4)          // SPI 2 Peek


