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

#define SH_AUXMUIIR(x)    (((x)&0x3)<<1)                // Shift the bits for this field


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

#define SH_AUXMUCTLRTS(x)   (((x)&0x3)<<4)              // adjust RTS-AUTO to the right bits


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

#define SH_AUXMUSTTX(x)     (((x)&0xf)<<24)             // adjust TXLVL to the right bits
#define SH_AUXMUSTRX(x)     (((x)&0xf)<<16)             // adjust RXLVL to the right bits


#define AUX_MU_BAUD_REG     (HW_BASE+0x215068)          // Mini UART Baudrate
//-------------------------------------------------------------------------------------------------------------------
#define AUXMU_BAUD          (0xffff)                    // Baud Rate


//
// -- The auxiliary SPI 1
//    -------------------
#define AUX_SPI0_CNTL0_REG  (HW_BASE+0x215080)          // SPI 1 Control register 0
//-------------------------------------------------------------------------------------------------------------------
#define AUXSPI0CTL0_SPD     (0xfff<<20)                 // Speed Control (System Clock Freq/(2 * (SPD+1)))
#define AUXSPI0CTL0_CHP     (0x7<<17)                   // Chip Select
#define AUXSPI0CTL0_PIM     (1<<16)                     // Post Input Mode
#define AUXSPI0CTL0_VCS     (1<<15)                     // Variable Chip Sel (1=TX FIFO; 0=CHP)
#define AUXSPI0CTL0_VWID    (1<<14)                     // Variable Width (1=TX FIFO; 0=SHL)
#define AUXSPI0CTL0_DOUT    (3<<12)                     // Extra Hold clk cycles (00=None; 01=1clk; 10=4clk; 11=7clk)
#define AUXSPI0CTL0_ENB     (1<<11)                     // Enabled
#define AUXSPI0CTL0_IRIS    (1<<10)                     // in: data clocked on (1=rising; 0=falling) edge of cycle
#define AUXSPI0CTL0_CLR     (1<<9)                      // Clear FIFO queues
#define AUXSPI0CTL0_ORIS    (1<<8)                      // out: data clocked on (1=rising; 0=falling) edge of cycle
#define AUXSPI0CTL0_INV     (1<<7)                      // invert clock (1=idle high)
#define AUXSPI0CTL0_SBIT    (1<<6)                      // shift out starting with (1=MS; 0=LS) bit first
#define AUXSPI0CTL0_SHL     (0x3f)                      // Shift length

#define SH_AUXSPI0CTL0SPD(x) (((x)&0xfff)<<20)          // shift to the correct position
#define SH_AUXSPI0CTL0CHP(x) (((x)&0x7)<<17)            // shift to the correct position


#define AUX_SPI0_CNTL1_REG  (HW_BASE+0x215084)          // SPI 1 Control register 1
//-------------------------------------------------------------------------------------------------------------------
#define AUXSPI0CTL1_CSHI    (7<<8)                      // Additional clock cycles where CS is high
#define AUXSPI0CTL1_TXIRQ   (1<<7)                      // Set for IRQ when TX FIFO empty
#define AUXSPI0CTL1_DONE    (1<<6)                      // Set for IRQ when idle
#define AUXSPI0CTL1_SBIT    (1<<1)                      // shift in starting with (1=MS; 0=LS) bit first (CTL0: out)
#define AUXSPI0CTL1_KEEP    (1<<0)                      // set to keep input

#define SH_AUXSPI0CTL1CSHI(x) (((x)&0x7)<<8)            // shift to the correct position


#define AUX_SPI0_STAT_REG   (HW_BASE+0x215088)          // SPI 1 Status
//-------------------------------------------------------------------------------------------------------------------
#define AUXSPI0STAT_TX      (0xff<<24)                  // Number of units in TX FIFO
#define AUXSPI0STAT_RX      (0xfff<<12)                 // Number of units in RX FIFO
#define AUXSPI0STAT_TXFULL  (1<<9)                      // TX FIFO Full
#define AUXSPI0STAT_TXEMPTY (1<<8)                      // TX FIFO Empty
#define AUXSPI0STAT_RXEMPTY (1<<7)                      // RX FIFO Empty
#define AUXSPI0STAT_BUSY    (1<<6)                      // Transferring Data
#define AUXSPI0STAT_BCNT    (0x3f)                      // Bit Count to process

#define SH_AUXSPI0STATTX(x) (((x)&0xff)<<24)            // shift to the correct position
#define SH_AUXSPI0STATRX(x) (((x)&0xfff)<<12)           // shift to the correct position


#define AUX_SPI0_IO_REG     (HW_BASE+0x215090)          // SPI 1 Data
//-------------------------------------------------------------------------------------------------------------------
#define AUX_SPI0_IO_DATA    (0xffff)                    // This is the data portion


#define AUX_SPI0_PEEK_REG   (HW_BASE+0x215094)          // SPI 1 Peek
//-------------------------------------------------------------------------------------------------------------------
#define AUX_SPI0_PEEK_DATA  (0xffff)                    // This is the data portion


//
// -- The auxiliary SPI 2
//    -------------------
#define AUX_SPI1_CNTL0_REG  (HW_BASE+0x2150c0)          // SPI 2 Control register 0
//-------------------------------------------------------------------------------------------------------------------
#define AUXSPI1CTL0_SPD     (0xfff<<20)                 // Speed Control (System Clock Freq/(2 * (SPD+1)))
#define AUXSPI1CTL0_CHP     (0x7<<17)                   // Chip Select
#define AUXSPI1CTL0_PIM     (1<<16)                     // Post Input Mode
#define AUXSPI1CTL0_VCS     (1<<15)                     // Variable Chip Sel (1=TX FIFO; 0=CHP)
#define AUXSPI1CTL0_VWID    (1<<14)                     // Variable Width (1=TX FIFO; 0=SHL)
#define AUXSPI1CTL0_DOUT    (3<<12)                     // Extra Hold clk cycles (00=None; 01=1clk; 10=4clk; 11=7clk)
#define AUXSPI1CTL0_ENB     (1<<11)                     // Enabled
#define AUXSPI1CTL0_IRIS    (1<<10)                     // in: data clocked on (1=rising; 0=falling) edge of cycle
#define AUXSPI1CTL0_CLR     (1<<9)                      // Clear FIFO queues
#define AUXSPI1CTL0_ORIS    (1<<8)                      // out: data clocked on (1=rising; 0=falling) edge of cycle
#define AUXSPI1CTL0_INV     (1<<7)                      // invert clock (1=idle high)
#define AUXSPI1CTL0_SBIT    (1<<6)                      // shift out starting with (1=MS; 0=LS) bit first
#define AUXSPI1CTL0_SHL     (0x3f)                      // Shift length

#define SH_AUXSPI1CTL0SPD(x) (((x)&0xfff)<<20)          // shift to the correct position
#define SH_AUXSPI1CTL0CHP(x) (((x)&0x7)<<17)            // shift to the correct position


#define AUX_SPI1_CNTL1_REG  (HW_BASE+0x2150c4)          // SPI 2 Control register 1
//-------------------------------------------------------------------------------------------------------------------
#define AUXSPI1CTL1_CSHI    (7<<8)                      // Additional clock cycles where CS is high
#define AUXSPI1CTL1_TXIRQ   (1<<7)                      // Set for IRQ when TX FIFO empty
#define AUXSPI1CTL1_DONE    (1<<6)                      // Set for IRQ when idle
#define AUXSPI1CTL1_SBIT    (1<<1)                      // shift in starting with (1=MS; 0=LS) bit first (CTL0: out)
#define AUXSPI1CTL1_KEEP    (1<<0)                      // set to keep input

#define SH_AUXSPI1CTL1CSHI(x) (((x)&0x7)<<8)            // shift to the correct position


#define AUX_SPI1_STAT_REG   (HW_BASE+0x2150c8)          // SPI 2 Status
//-------------------------------------------------------------------------------------------------------------------
#define AUXSPI1STAT_TX      (0xff<<24)                  // Number of units in TX FIFO
#define AUXSPI1STAT_RX      (0xfff<<12)                 // Number of units in RX FIFO
#define AUXSPI1STAT_TXFULL  (1<<9)                      // TX FIFO Full
#define AUXSPI1STAT_TXEMPTY (1<<8)                      // TX FIFO Empty
#define AUXSPI1STAT_RXEMPTY (1<<7)                      // RX FIFO Empty
#define AUXSPI1STAT_BUSY    (1<<6)                      // Transferring Data
#define AUXSPI1STAT_BCNT    (0x3f)                      // Bit Count to process

#define SH_AUXSPI1STATTX(x) (((x)&0xff)<<24)            // shift to the correct position
#define SH_AUXSPI1STATRX(x) (((x)&0xfff)<<12)           // shift to the correct position


#define AUX_SPI1_IO_REG     (HW_BASE+0x2150d0)          // SPI 2 Data
//-------------------------------------------------------------------------------------------------------------------
#define AUX_SPI1_IO_DATA    (0xffff)                    // This is the data portion


#define AUX_SPI1_PEEK_REG   (HW_BASE+0x2150d4)          // SPI 2 Peek
//-------------------------------------------------------------------------------------------------------------------
#define AUX_SPI1_PEEK_DATA  (0xffff)                    // This is the data portion


//-------------------------------------------------------------------------------------------------------------------
// Broadcom Serial Controllers: BSC0 & BSC1 (note BSC2 should not be accessed by software)
//-------------------------------------------------------------------------------------------------------------------


//
// -- The BSC0 controller
//    -------------------
#define BSC0_BASE           (HW_BASE+0x205000)          // The base address of BSC0


#define BSC0_C              (BSC0_BASE)                 // Control
//-------------------------------------------------------------------------------------------------------------------
#define BSC0C_I2CEN         (1<<15)                     // I2C Enable
#define BSC0C_INTR          (1<<10)                     // Interrupt on RXR=1
#define BSC0C_INTT          (1<<9)                      // Interrupt on TXW=1
#define BSC0C_INTD          (1<<8)                      // Interrupt on DONE=1
#define BSC0C_ST            (1<<7)                      // Start new transfer
#define BSC0C_CLEAR         (3<<4)                      // Clear FIFO if either bit is set
#define BSC0C_READ          (1<<0)                      // Read Packet transfer

#define SH_BSC0C_CLEAR(x)   (((x)&3)<<4)                // shift bits to the right location


#define BSC0_S              (BSC0_BASE+4)               // Status
//-------------------------------------------------------------------------------------------------------------------
#define BSC0S_CLKT          (1<<9)                      // 1=Slave has held SCL too long;  Write 1 to clear
#define BSC0S_ERR           (1<<8)                      // 1=Slave has not ACK address;  Write 1 to clear
#define BSC0S_RXF           (1<<7)                      // 1=FIFO full
#define BSC0S_TXE           (1<<6)                      // 1=FIFO empty
#define BSC0S_RXD           (1<<5)                      // 1=FIFO has data
#define BSC0S_TXD           (1<<4)                      // 1=FIFO has room
#define BSC0S_RXR           (1<<3)                      // 1=FIFO full; read underway
#define BSC0S_TXW           (1<<2)                      // 1=FIFO fill; write underway
#define BSC0S_DONE          (1<<1)                      // 1=Transfer complete
#define BSC0S_TA            (1<<0)                      // 1=Transfer active


#define BSC0_DLEN           (BSC0_BASE+8)               // Data Length
//-------------------------------------------------------------------------------------------------------------------
#define BSC0DLEN_DLEN       (0xffff)                    // Data Length


#define BSC0_A              (BSC0_BASE+0xc)             // Slave Address
//-------------------------------------------------------------------------------------------------------------------
#define BSC0A_ADDR          (0xffff)                    // Slave Address


#define BSC0_FIFO           (BSC0_BASE+0x10)            // Data FIFO
//-------------------------------------------------------------------------------------------------------------------
#define BSC0FIFO_DATA       (0xffff)                    // Data


#define BSC0_DIV            (BSC0_BASE+0x14)            // Clock Divider
//-------------------------------------------------------------------------------------------------------------------
#define BSC0DIV_CDIV        (0xffff)                    // Clock Divider


#define BSC0_DEL            (BSC0_BASE+0x18)            // Data Delay
//-------------------------------------------------------------------------------------------------------------------
#define BSC0DEL_FEDL        (0xffff<<16)                // Falling Edge Delay
#define BSC0DEL_REDL        (0xffff)                    // Rising Edge Delay

#define SH_BSC0DELFEDL(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define BSC0_CLKT           (BSC0_BASE+0x1c)            // Clock Stretch Timeout
//-------------------------------------------------------------------------------------------------------------------
#define BSC0CLKT_TOUT       (0xffff)                    // Clock Stretch Timeout value


//
// -- The BSC1 controller
//    -------------------
#define BSC1_BASE           (HW_BASE+0x804000)          // The base address of BSC1


#define BSC1_C              (BSC1_BASE)                 // Control
//-------------------------------------------------------------------------------------------------------------------
#define BSC1C_I2CEN         (1<<15)                     // I2C Enable
#define BSC1C_INTR          (1<<10)                     // Interrupt on RXR=1
#define BSC1C_INTT          (1<<9)                      // Interrupt on TXW=1
#define BSC1C_INTD          (1<<8)                      // Interrupt on DONE=1
#define BSC1C_ST            (1<<7)                      // Start new transfer
#define BSC1C_CLEAR         (3<<4)                      // Clear FIFO if either bit is set
#define BSC1C_READ          (1<<0)                      // Read Packet transfer

#define SH_BSC1C_CLEAR(x)   (((x)&3)<<4)                // shift bits to the right location


#define BSC1_S              (BSC1_BASE+4)               // Status
//-------------------------------------------------------------------------------------------------------------------
#define BSC1S_CLKT          (1<<9)                      // 1=Slave has held SCL too long;  Write 1 to clear
#define BSC1S_ERR           (1<<8)                      // 1=Slave has not ACK address;  Write 1 to clear
#define BSC1S_RXF           (1<<7)                      // 1=FIFO full
#define BSC1S_TXE           (1<<6)                      // 1=FIFO empty
#define BSC1S_RXD           (1<<5)                      // 1=FIFO has data
#define BSC1S_TXD           (1<<4)                      // 1=FIFO has room
#define BSC1S_RXR           (1<<3)                      // 1=FIFO full; read underway
#define BSC1S_TXW           (1<<2)                      // 1=FIFO fill; write underway
#define BSC1S_DONE          (1<<1)                      // 1=Transfer complete
#define BSC1S_TA            (1<<0)                      // 1=Transfer active


#define BSC1_DLEN           (BSC1_BASE+8)               // Data Length
//-------------------------------------------------------------------------------------------------------------------
#define BSC1DLEN_DLEN       (0xffff)                    // Data Length


#define BSC1_A              (BSC1_BASE+0xc)             // Slave Address
//-------------------------------------------------------------------------------------------------------------------
#define BSC1A_ADDR          (0xffff)                    // Slave Address


#define BSC1_FIFO           (BSC1_BASE+0x10)            // Data FIFO
//-------------------------------------------------------------------------------------------------------------------
#define BSC1FIFO_DATA       (0xffff)                    // Data


#define BSC1_DIV            (BSC1_BASE+0x14)            // Clock Divider
//-------------------------------------------------------------------------------------------------------------------
#define BSC1DIV_CDIV        (0xffff)                    // Clock Divider


#define BSC1_DEL            (BSC1_BASE+0x18)            // Data Delay
//-------------------------------------------------------------------------------------------------------------------
#define BSC1DEL_FEDL        (0xffff<<16)                // Falling Edge Delay
#define BSC1DEL_REDL        (0xffff)                    // Rising Edge Delay

#define SH_BSC1DELFEDL(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define BSC1_CLKT           (BSC1_BASE+0x1c)            // Clock Stretch Timeout
//-------------------------------------------------------------------------------------------------------------------
#define BSC1CLKT_TOUT       (0xffff)                    // Clock Stretch Timeout value

