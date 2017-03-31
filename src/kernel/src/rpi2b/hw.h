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


//-------------------------------------------------------------------------------------------------------------------
// DMA Controllers DMA0-DMA15
//-------------------------------------------------------------------------------------------------------------------

//
// -- The DMA0 controller
//    -------------------
#define DMA0_BASE           (HW_BASE+0x7000)            // Base Address of DMA0


#define DMA0_CS             (DMA0_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA0CS_RESET        (1<<31)                     // reset DMA
#define DMA0CS_ABORT        (1<<30)                     // abort current CB
#define DMA0CS_DISDBG       (1<<29)                     // disable debug
#define DMA0CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA0CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA0CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA0CS_ERROR        (1<<8)                      // Channel has error
#define DMA0CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA0CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA0CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA0CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA0CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA0CS_END          (1<<1)                      // 1 when complete
#define DMA0CS_ACTIVE       (1<<0)                      // active

#define SH_DMA0CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA0CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA0_CBAD           (DMA0_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA0CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA0_TI             (DMA0_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA0TI_NOWIDE       (1<<26)                     // Do not do write as 2 beat bursts
#define DMA0TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA0TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA0TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA0TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA0TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA0TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA0TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA0TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA0TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA0TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA0TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA0TI_WRESP        (1<<3)                      // Wait for Response
#define DMA0TI_TMODE        (1<<1)                      // 2D Mode
#define DMA0TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA0TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA0TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA0_SRCAD          (DMA0_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA0SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA0_DSTAD          (DMA0_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA0DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA0_XLEN           (DMA0_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA0XLEN_Y          (0x3fff<<16)                // in 2D mode, number of DMA0XLEN_X
#define DMA0XLEN_X          (0xffff)                    // Transfer bytes length

#define SH_DMA0XLENY(x)     (((x)&0x3fff)<<16)          // shift the value to the correct position


#define DMA0_STRIDE         (DMA0_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA0STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA0STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA0STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA0_NXTCB          (DMA0_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA0NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA0_DEBUG          (DMA0_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA0DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA0DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA0DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA0DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA0DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA0DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA0DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA0DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA1 controller
//    -------------------
#define DMA1_BASE           (DMA1_BASE+0x100)           // Base Address of DMA1
//-------------------------------------------------------------------------------------------------------------------


#define DMA1_CS             (DMA1_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA1CS_RESET        (1<<31)                     // reset DMA
#define DMA1CS_ABORT        (1<<30)                     // abort current CB
#define DMA1CS_DISDBG       (1<<29)                     // disable debug
#define DMA1CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA1CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA1CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA1CS_ERROR        (1<<8)                      // Channel has error
#define DMA1CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA1CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA1CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA1CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA1CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA1CS_END          (1<<1)                      // 1 when complete
#define DMA1CS_ACTIVE       (1<<0)                      // active

#define SH_DMA1CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA1CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA1_CBAD           (DMA1_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA1CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA1_TI             (DMA1_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA1TI_NOWIDE       (1<<26)                     // Do not do write as 2 beat bursts
#define DMA1TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA1TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA1TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA1TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA1TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA1TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA1TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA1TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA1TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA1TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA1TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA1TI_WRESP        (1<<3)                      // Wait for Response
#define DMA1TI_TMODE        (1<<1)                      // 2D Mode
#define DMA1TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA1TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA1TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA1_SRCAD          (DMA1_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA1SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA1_DSTAD          (DMA1_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA1DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA1_XLEN           (DMA1_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA1XLEN_Y          (0x3fff<<16)                // in 2D mode, number of DMA0XLEN_X
#define DMA1XLEN_X          (0xffff)                    // Transfer bytes length

#define SH_DMA1XLENY(x)     (((x)&0x3fff)<<16)          // shift the value to the correct position


#define DMA1_STRIDE         (DMA1_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA1STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA1STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA1STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA1_NXTCB          (DMA1_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA1NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA1_DEBUG          (DMA1_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA1DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA1DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA1DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA1DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA1DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA1DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA1DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA1DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA2 controller
//    -------------------
#define DMA2_BASE           (DMA1_BASE+0x100)           // Base Address of DMA2
//-------------------------------------------------------------------------------------------------------------------


#define DMA2_CS             (DMA2_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA2CS_RESET        (1<<31)                     // reset DMA
#define DMA2CS_ABORT        (1<<30)                     // abort current CB
#define DMA2CS_DISDBG       (1<<29)                     // disable debug
#define DMA2CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA2CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA2CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA2CS_ERROR        (1<<8)                      // Channel has error
#define DMA2CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA2CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA2CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA2CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA2CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA2CS_END          (1<<1)                      // 1 when complete
#define DMA2CS_ACTIVE       (1<<0)                      // active

#define SH_DMA2CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA2CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA2_CBAD           (DMA2_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA2CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA2_TI             (DMA2_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA2TI_NOWIDE       (1<<26)                     // Do not do write as 2 beat bursts
#define DMA2TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA2TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA2TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA2TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA2TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA2TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA2TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA2TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA2TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA2TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA2TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA2TI_WRESP        (1<<3)                      // Wait for Response
#define DMA2TI_TMODE        (1<<1)                      // 2D Mode
#define DMA2TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA2TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA2TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA2_SRCAD          (DMA2_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA2SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA2_DSTAD          (DMA2_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA2DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA2_XLEN           (DMA2_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA2XLEN_Y          (0x3fff<<16)                // in 2D mode, number of DMA0XLEN_X
#define DMA2XLEN_X          (0xffff)                    // Transfer bytes length

#define SH_DMA2XLENY(x)     (((x)&0x3fff)<<16)          // shift the value to the correct position


#define DMA2_STRIDE         (DMA2_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA2STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA2STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA2STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA2_NXTCB          (DMA2_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA2NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA2_DEBUG          (DMA2_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA2DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA2DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA2DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA2DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA2DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA2DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA2DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA2DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA3 controller
//    -------------------
#define DMA3_BASE           (DMA2_BASE+0x100)           // Base Address of DMA4
//-------------------------------------------------------------------------------------------------------------------


#define DMA3_CS             (DMA3_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA3CS_RESET        (1<<31)                     // reset DMA
#define DMA3CS_ABORT        (1<<30)                     // abort current CB
#define DMA3CS_DISDBG       (1<<29)                     // disable debug
#define DMA3CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA3CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA3CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA3CS_ERROR        (1<<8)                      // Channel has error
#define DMA3CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA3CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA3CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA3CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA3CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA3CS_END          (1<<1)                      // 1 when complete
#define DMA3CS_ACTIVE       (1<<0)                      // active

#define SH_DMA3CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA3CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA3_CBAD           (DMA3_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA3CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA3_TI             (DMA3_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA3TI_NOWIDE       (1<<26)                     // Do not do write as 2 beat bursts
#define DMA3TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA3TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA3TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA3TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA3TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA3TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA3TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA3TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA3TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA3TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA3TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA3TI_WRESP        (1<<3)                      // Wait for Response
#define DMA3TI_TMODE        (1<<1)                      // 2D Mode
#define DMA3TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA3TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA3TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA3_SRCAD          (DMA3_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA3SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA3_DSTAD          (DMA3_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA3DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA3_XLEN           (DMA3_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA3XLEN_Y          (0x3fff<<16)                // in 2D mode, number of DMA0XLEN_X
#define DMA3XLEN_X          (0xffff)                    // Transfer bytes length

#define SH_DMA3XLENY(x)     (((x)&0x3fff)<<16)          // shift the value to the correct position


#define DMA3_STRIDE         (DMA3_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA3STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA3STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA3STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA3_NXTCB          (DMA3_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA3NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA3_DEBUG          (DMA3_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA3DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA3DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA3DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA3DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA3DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA3DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA3DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA3DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA4 controller
//    -------------------
#define DMA4_BASE           (DMA3_BASE+0x100)           // Base Address of DMA4
//-------------------------------------------------------------------------------------------------------------------


#define DMA4_CS             (DMA4_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA4CS_RESET        (1<<31)                     // reset DMA
#define DMA4CS_ABORT        (1<<30)                     // abort current CB
#define DMA4CS_DISDBG       (1<<29)                     // disable debug
#define DMA4CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA4CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA4CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA4CS_ERROR        (1<<8)                      // Channel has error
#define DMA4CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA4CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA4CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA4CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA4CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA4CS_END          (1<<1)                      // 1 when complete
#define DMA4CS_ACTIVE       (1<<0)                      // active

#define SH_DMA4CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA4CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA4_CBAD           (DMA4_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA4CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA4_TI             (DMA4_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA4TI_NOWIDE       (1<<26)                     // Do not do write as 2 beat bursts
#define DMA4TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA4TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA4TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA4TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA4TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA4TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA4TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA4TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA4TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA4TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA4TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA4TI_WRESP        (1<<3)                      // Wait for Response
#define DMA4TI_TMODE        (1<<1)                      // 2D Mode
#define DMA4TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA4TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA4TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA4_SRCAD          (DMA4_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA4SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA4_DSTAD          (DMA4_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA4DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA4_XLEN           (DMA4_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA4XLEN_Y          (0x3fff<<16)                // in 2D mode, number of DMA0XLEN_X
#define DMA4XLEN_X          (0xffff)                    // Transfer bytes length

#define SH_DMA4XLENY(x)     (((x)&0x3fff)<<16)          // shift the value to the correct position


#define DMA4_STRIDE         (DMA4_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA4STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA4STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA4STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA4_NXTCB          (DMA4_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA4NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA4_DEBUG          (DMA4_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA4DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA4DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA4DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA4DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA4DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA4DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA4DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA4DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA5 controller
//    -------------------
#define DMA5_BASE           (DMA4_BASE+0x100)           // Base Address of DMA5
//-------------------------------------------------------------------------------------------------------------------


#define DMA5_CS             (DMA5_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA5CS_RESET        (1<<31)                     // reset DMA
#define DMA5CS_ABORT        (1<<30)                     // abort current CB
#define DMA5CS_DISDBG       (1<<29)                     // disable debug
#define DMA5CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA5CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA5CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA5CS_ERROR        (1<<8)                      // Channel has error
#define DMA5CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA5CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA5CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA5CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA5CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA5CS_END          (1<<1)                      // 1 when complete
#define DMA5CS_ACTIVE       (1<<0)                      // active

#define SH_DMA5CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA5CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA5_CBAD           (DMA5_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA5CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA5_TI             (DMA5_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA5TI_NOWIDE       (1<<26)                     // Do not do write as 2 beat bursts
#define DMA5TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA5TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA5TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA5TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA5TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA5TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA5TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA5TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA5TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA5TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA5TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA5TI_WRESP        (1<<3)                      // Wait for Response
#define DMA5TI_TMODE        (1<<1)                      // 2D Mode
#define DMA5TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA5TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA5TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA5_SRCAD          (DMA5_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA5SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA5_DSTAD          (DMA5_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA5DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA5_XLEN           (DMA5_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA5XLEN_Y          (0x3fff<<16)                // in 2D mode, number of DMA0XLEN_X
#define DMA5XLEN_X          (0xffff)                    // Transfer bytes length

#define SH_DMA5XLENY(x)     (((x)&0x3fff)<<16)          // shift the value to the correct position


#define DMA5_STRIDE         (DMA5_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA5STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA5STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA5STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA5_NXTCB          (DMA5_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA5NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA5_DEBUG          (DMA5_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA5DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA5DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA5DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA5DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA5DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA5DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA5DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA5DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA6 controller
//    -------------------
#define DMA6_BASE           (DMA5_BASE+0x100)           // Base Address of DMA6
//-------------------------------------------------------------------------------------------------------------------


#define DMA6_CS             (DMA6_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA6CS_RESET        (1<<31)                     // reset DMA
#define DMA6CS_ABORT        (1<<30)                     // abort current CB
#define DMA6CS_DISDBG       (1<<29)                     // disable debug
#define DMA6CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA6CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA6CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA6CS_ERROR        (1<<8)                      // Channel has error
#define DMA6CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA6CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA6CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA6CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA6CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA6CS_END          (1<<1)                      // 1 when complete
#define DMA6CS_ACTIVE       (1<<0)                      // active

#define SH_DMA6CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA6CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA6_CBAD           (DMA6_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA6CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA6_TI             (DMA6_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA6TI_NOWIDE       (1<<26)                     // Do not do write as 2 beat bursts
#define DMA6TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA6TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA6TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA6TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA6TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA6TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA6TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA6TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA6TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA6TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA6TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA6TI_WRESP        (1<<3)                      // Wait for Response
#define DMA6TI_TMODE        (1<<1)                      // 2D Mode
#define DMA6TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA6TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA6TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA6_SRCAD          (DMA6_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA6SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA6_DSTAD          (DMA6_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA6DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA6_XLEN           (DMA6_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA6XLEN_Y          (0x3fff<<16)                // in 2D mode, number of DMA0XLEN_X
#define DMA6XLEN_X          (0xffff)                    // Transfer bytes length

#define SH_DMA6XLENY(x)     (((x)&0x3fff)<<16)          // shift the value to the correct position


#define DMA6_STRIDE         (DMA6_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA6STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA6STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA6STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA6_NXTCB          (DMA6_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA6NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA6_DEBUG          (DMA6_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA6DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA6DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA6DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA6DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA6DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA6DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA6DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA6DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA7 controller
//    -------------------
#define DMA7_BASE           (DMA6_BASE+0x100)           // Base Address of DMA7
//-------------------------------------------------------------------------------------------------------------------


#define DMA7_CS             (DMA7_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA7CS_RESET        (1<<31)                     // reset DMA
#define DMA7CS_ABORT        (1<<30)                     // abort current CB
#define DMA7CS_DISDBG       (1<<29)                     // disable debug
#define DMA7CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA7CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA7CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA7CS_ERROR        (1<<8)                      // Channel has error
#define DMA7CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA7CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA7CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA7CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA7CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA7CS_END          (1<<1)                      // 1 when complete
#define DMA7CS_ACTIVE       (1<<0)                      // active

#define SH_DMA7CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA7CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA7_CBAD           (DMA7_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA7CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA7_TI             (DMA7_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA7TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA7TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA7TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA7TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA7TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA7TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA7TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA7TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA7TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA7TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA7TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA7TI_WRESP        (1<<3)                      // Wait for Response
#define DMA7TI_TMODE        (1<<1)                      // 2D Mode
#define DMA7TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA7TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA7TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA7_SRCAD          (DMA7_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA7SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA7_DSTAD          (DMA7_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA7DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA7_XLEN           (DMA7_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA7XLEN_X          (0xffff)                    // Transfer bytes length


#define DMA7_STRIDE         (DMA7_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA7STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA7STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA7STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA7_NXTCB          (DMA7_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA7NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA7_DEBUG          (DMA7_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA7DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA7DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA7DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA7DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA7DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA7DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA7DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA7DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA8 controller
//    -------------------
#define DMA8_BASE           (DMA7_BASE+0x100)           // Base Address of DMA8
//-------------------------------------------------------------------------------------------------------------------


#define DMA8_CS             (DMA8_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA8CS_RESET        (1<<31)                     // reset DMA
#define DMA8CS_ABORT        (1<<30)                     // abort current CB
#define DMA8CS_DISDBG       (1<<29)                     // disable debug
#define DMA8CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA8CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA8CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA8CS_ERROR        (1<<8)                      // Channel has error
#define DMA8CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA8CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA8CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA8CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA8CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA8CS_END          (1<<1)                      // 1 when complete
#define DMA8CS_ACTIVE       (1<<0)                      // active

#define SH_DMA8CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA8CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA8_CBAD           (DMA8_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA8CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA8_TI             (DMA8_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA8TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA8TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA8TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA8TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA8TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA8TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA8TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA8TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA8TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA8TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA8TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA8TI_WRESP        (1<<3)                      // Wait for Response
#define DMA8TI_TMODE        (1<<1)                      // 2D Mode
#define DMA8TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA8TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA8TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA8_SRCAD          (DMA8_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA8SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA8_DSTAD          (DMA8_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA8DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA8_XLEN           (DMA8_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA8XLEN_X          (0xffff)                    // Transfer bytes length


#define DMA8_STRIDE         (DMA8_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA8STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA8STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA8STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA8_NXTCB          (DMA8_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA8NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA8_DEBUG          (DMA8_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA8DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA8DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA8DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA8DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA8DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA8DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA8DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA8DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA9 controller
//    -------------------
#define DMA9_BASE           (DMA8_BASE+0x100)           // Base Address of DMA9
//-------------------------------------------------------------------------------------------------------------------


#define DMA9_CS             (DMA9_BASE)                 // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA9CS_RESET        (1<<31)                     // reset DMA
#define DMA9CS_ABORT        (1<<30)                     // abort current CB
#define DMA9CS_DISDBG       (1<<29)                     // disable debug
#define DMA9CS_WAIT         (1<<28)                     // wait for outstanding writes
#define DMA9CS_PANICPTY     (0xf<<20)                   // priority of panicking transactions
#define DMA9CS_PTY          (0xf<<16)                   // priority of transactions
#define DMA9CS_ERROR        (1<<8)                      // Channel has error
#define DMA9CS_WAITING      (1<<6)                      // is waiting for outstanding writes
#define DMA9CS_REQSTOP      (1<<5)                      // DREQ stops DMA
#define DMA9CS_PAUSE        (1<<4)                      // DMA is paused
#define DMA9CS_DREQ         (1<<3)                      // 1=requesting data
#define DMA9CS_INT          (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA9CS_END          (1<<1)                      // 1 when complete
#define DMA9CS_ACTIVE       (1<<0)                      // active

#define SH_DMA9CSPANIC(x)   (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA9CSPTY(x)     (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA9_CBAD           (DMA9_BASE+4)               // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA9CBAD_SCB        (0xffffffff)                // Source Control Block address


#define DMA9_TI             (DMA9_BASE+8)               // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA9TI_WAITS        (0x1f<<21)                  // Add wait cycles
#define DMA9TI_PMAP         (0x1f<<16)                  // peripheral map
#define DMA9TI_BLEN         (0xf<<12)                   // burst transfer length
#define DMA9TI_SRCIGN       (1<<11)                     // Ignore Reads
#define DMA9TI_SRCDREQ      (1<<10)                     // Control source reads with DREQ
#define DMA9TI_SRCWID       (1<<9)                      // Source Transfer Width
#define DMA9TI_SRCINC       (1<<8)                      // Source Address Increment
#define DMA9TI_DSTIGN       (1<<7)                      // Ignore Writes
#define DMA9TI_DSTDREQ      (1<<6)                      // Control source writes with DREQ
#define DMA9TI_DSTWID       (1<<5)                      // Dest Transfer Width
#define DMA9TI_DSTINC       (1<<4)                      // Dest Address Increment
#define DMA9TI_WRESP        (1<<3)                      // Wait for Response
#define DMA9TI_TMODE        (1<<1)                      // 2D Mode
#define DMA9TI_INTEN        (1<<0)                      // Interrupt Enable

#define SH_DMA9TIWAITS(x)   (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA9TIPMAP(x)    (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA9_SRCAD          (DMA9_BASE+0xc)             // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA9SRCAD_SRC       (0xffffffff)                // DMA Source address


#define DMA9_DSTAD          (DMA9_BASE+0x10)            // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA9DSTAD_SRC       (0xffffffff)                // DMA Dest address


#define DMA9_XLEN           (DMA9_BASE+0x14)            // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA9XLEN_X          (0xffff)                    // Transfer bytes length


#define DMA9_STRIDE         (DMA9_BASE+0x18)            // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA9STRIDE_D        (0xffff<<16)                // Destination Stride
#define DMA9STRIDE_S        (0xffff)                    // Source Stride

#define SH_DMA9STRIDED(x)   (((x)&0xffff)<<16)          // shift to the correct position


#define DMA9_NXTCB          (DMA9_BASE+0x1c)            // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA9NXTCB_ADD       (0xffffffff)                // NExt Control Block Address


#define DMA9_DEBUG          (DMA9_BASE+0x20)            // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA9DEBUG_LITE      (1<<28)                     // DMA Lite
#define DMA9DEBUG_VERS      (7<<25)                     // DMA Version
#define DMA9DEBUG_STATE     (0x1ff<<16)                 // DMA State
#define DMA9DEBUG_ID        (0xff<<8)                   // DMA ID
#define DMA9DEBUG_OWRITE    (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA9DEBUG_RERROR    (1<<2)                      // Slave read response error
#define DMA9DEBUG_FERROR    (1<<1)                      // FIFO error
#define DMA9DEBUG_LAST      (1<<0)                      // Read Last Not Set Error


//
// -- The DMA10 controller
//    --------------------
#define DMA10_BASE          (DMA9_BASE+0x100)           // Base Address of DMA10
//-------------------------------------------------------------------------------------------------------------------


#define DMA10_CS            (DMA10_BASE)                // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA10CS_RESET       (1<<31)                     // reset DMA
#define DMA10CS_ABORT       (1<<30)                     // abort current CB
#define DMA10CS_DISDBG      (1<<29)                     // disable debug
#define DMA10CS_WAIT        (1<<28)                     // wait for outstanding writes
#define DMA10CS_PANICPTY    (0xf<<20)                   // priority of panicking transactions
#define DMA10CS_PTY         (0xf<<16)                   // priority of transactions
#define DMA10CS_ERROR       (1<<8)                      // Channel has error
#define DMA10CS_WAITING     (1<<6)                      // is waiting for outstanding writes
#define DMA10CS_REQSTOP     (1<<5)                      // DREQ stops DMA
#define DMA10CS_PAUSE       (1<<4)                      // DMA is paused
#define DMA10CS_DREQ        (1<<3)                      // 1=requesting data
#define DMA10CS_INT         (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA10CS_END         (1<<1)                      // 1 when complete
#define DMA10CS_ACTIVE      (1<<0)                      // active

#define SH_DMA10CSPANIC(x)  (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA10CSPTY(x)    (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA10_CBAD          (DMA10_BASE+4)              // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA10CBAD_SCB       (0xffffffff)                // Source Control Block address


#define DMA10_TI            (DMA10_BASE+8)              // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA10TI_WAITS       (0x1f<<21)                  // Add wait cycles
#define DMA10TI_PMAP        (0x1f<<16)                  // peripheral map
#define DMA10TI_BLEN        (0xf<<12)                   // burst transfer length
#define DMA10TI_SRCIGN      (1<<11)                     // Ignore Reads
#define DMA10TI_SRCDREQ     (1<<10)                     // Control source reads with DREQ
#define DMA10TI_SRCWID      (1<<9)                      // Source Transfer Width
#define DMA10TI_SRCINC      (1<<8)                      // Source Address Increment
#define DMA10TI_DSTIGN      (1<<7)                      // Ignore Writes
#define DMA10TI_DSTDREQ     (1<<6)                      // Control source writes with DREQ
#define DMA10TI_DSTWID      (1<<5)                      // Dest Transfer Width
#define DMA10TI_DSTINC      (1<<4)                      // Dest Address Increment
#define DMA10TI_WRESP       (1<<3)                      // Wait for Response
#define DMA10TI_TMODE       (1<<1)                      // 2D Mode
#define DMA10TI_INTEN       (1<<0)                      // Interrupt Enable

#define SH_DMA10TIWAITS(x)  (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA10TIPMAP(x)   (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA10_SRCAD         (DMA10_BASE+0xc)            // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA10SRCAD_SRC      (0xffffffff)                // DMA Source address


#define DMA10_DSTAD         (DMA10_BASE+0x10)           // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA10DSTAD_SRC      (0xffffffff)                // DMA Dest address


#define DMA10_XLEN          (DMA10_BASE+0x14)           // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA10XLEN_X         (0xffff)                    // Transfer bytes length


#define DMA10_STRIDE        (DMA10_BASE+0x18)           // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA10STRIDE_D       (0xffff<<16)                // Destination Stride
#define DMA10STRIDE_S       (0xffff)                    // Source Stride

#define SH_DMA10STRIDED(x)  (((x)&0xffff)<<16)          // shift to the correct position


#define DMA10_NXTCB         (DMA10_BASE+0x1c)           // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA10NXTCB_ADD      (0xffffffff)                // NExt Control Block Address


#define DMA10_DEBUG         (DMA10_BASE+0x20)           // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA10DEBUG_LITE     (1<<28)                     // DMA Lite
#define DMA10DEBUG_VERS     (7<<25)                     // DMA Version
#define DMA10DEBUG_STATE    (0x1ff<<16)                 // DMA State
#define DMA10DEBUG_ID       (0xff<<8)                   // DMA ID
#define DMA10DEBUG_OWRITE   (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA10DEBUG_RERROR   (1<<2)                      // Slave read response error
#define DMA10DEBUG_FERROR   (1<<1)                      // FIFO error
#define DMA10DEBUG_LAST     (1<<0)                      // Read Last Not Set Error


//
// -- The DMA11 controller
//    --------------------
#define DMA11_BASE          (DMA10_BASE+0x100)          // Base Address of DMA11
//-------------------------------------------------------------------------------------------------------------------


#define DMA11_CS            (DMA11_BASE)                // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA11CS_RESET       (1<<31)                     // reset DMA
#define DMA11CS_ABORT       (1<<30)                     // abort current CB
#define DMA11CS_DISDBG      (1<<29)                     // disable debug
#define DMA11CS_WAIT        (1<<28)                     // wait for outstanding writes
#define DMA11CS_PANICPTY    (0xf<<20)                   // priority of panicking transactions
#define DMA11CS_PTY         (0xf<<16)                   // priority of transactions
#define DMA11CS_ERROR       (1<<8)                      // Channel has error
#define DMA11CS_WAITING     (1<<6)                      // is waiting for outstanding writes
#define DMA11CS_REQSTOP     (1<<5)                      // DREQ stops DMA
#define DMA11CS_PAUSE       (1<<4)                      // DMA is paused
#define DMA11CS_DREQ        (1<<3)                      // 1=requesting data
#define DMA11CS_INT         (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA11CS_END         (1<<1)                      // 1 when complete
#define DMA11CS_ACTIVE      (1<<0)                      // active

#define SH_DMA11CSPANIC(x)  (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA11CSPTY(x)    (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA11_CBAD          (DMA11_BASE+4)              // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA11CBAD_SCB       (0xffffffff)                // Source Control Block address


#define DMA11_TI            (DMA11_BASE+8)              // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA11TI_WAITS       (0x1f<<21)                  // Add wait cycles
#define DMA11TI_PMAP        (0x1f<<16)                  // peripheral map
#define DMA11TI_BLEN        (0xf<<12)                   // burst transfer length
#define DMA11TI_SRCIGN      (1<<11)                     // Ignore Reads
#define DMA11TI_SRCDREQ     (1<<10)                     // Control source reads with DREQ
#define DMA11TI_SRCWID      (1<<9)                      // Source Transfer Width
#define DMA11TI_SRCINC      (1<<8)                      // Source Address Increment
#define DMA11TI_DSTIGN      (1<<7)                      // Ignore Writes
#define DMA11TI_DSTDREQ     (1<<6)                      // Control source writes with DREQ
#define DMA11TI_DSTWID      (1<<5)                      // Dest Transfer Width
#define DMA11TI_DSTINC      (1<<4)                      // Dest Address Increment
#define DMA11TI_WRESP       (1<<3)                      // Wait for Response
#define DMA11TI_TMODE       (1<<1)                      // 2D Mode
#define DMA11TI_INTEN       (1<<0)                      // Interrupt Enable

#define SH_DMA11TIWAITS(x)  (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA11TIPMAP(x)   (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA11_SRCAD         (DMA11_BASE+0xc)            // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA11SRCAD_SRC      (0xffffffff)                // DMA Source address


#define DMA11_DSTAD         (DMA11_BASE+0x10)           // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA11DSTAD_SRC      (0xffffffff)                // DMA Dest address


#define DMA11_XLEN          (DMA11_BASE+0x14)           // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA11XLEN_X         (0xffff)                    // Transfer bytes length


#define DMA11_STRIDE        (DMA11_BASE+0x18)           // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA11STRIDE_D       (0xffff<<16)                // Destination Stride
#define DMA11STRIDE_S       (0xffff)                    // Source Stride

#define SH_DMA11STRIDED(x)  (((x)&0xffff)<<16)          // shift to the correct position


#define DMA11_NXTCB         (DMA11_BASE+0x1c)           // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA11NXTCB_ADD      (0xffffffff)                // NExt Control Block Address


#define DMA11_DEBUG         (DMA11_BASE+0x20)           // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA11DEBUG_LITE     (1<<28)                     // DMA Lite
#define DMA11DEBUG_VERS     (7<<25)                     // DMA Version
#define DMA11DEBUG_STATE    (0x1ff<<16)                 // DMA State
#define DMA11DEBUG_ID       (0xff<<8)                   // DMA ID
#define DMA11DEBUG_OWRITE   (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA11DEBUG_RERROR   (1<<2)                      // Slave read response error
#define DMA11DEBUG_FERROR   (1<<1)                      // FIFO error
#define DMA11DEBUG_LAST     (1<<0)                      // Read Last Not Set Error


//
// -- The DMA12 controller
//    --------------------
#define DMA12_BASE          (DMA11_BASE+0x100)          // Base Address of DMA12
//-------------------------------------------------------------------------------------------------------------------


#define DMA12_CS            (DMA12_BASE)                // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA12CS_RESET       (1<<31)                     // reset DMA
#define DMA12CS_ABORT       (1<<30)                     // abort current CB
#define DMA12CS_DISDBG      (1<<29)                     // disable debug
#define DMA12CS_WAIT        (1<<28)                     // wait for outstanding writes
#define DMA12CS_PANICPTY    (0xf<<20)                   // priority of panicking transactions
#define DMA12CS_PTY         (0xf<<16)                   // priority of transactions
#define DMA12CS_ERROR       (1<<8)                      // Channel has error
#define DMA12CS_WAITING     (1<<6)                      // is waiting for outstanding writes
#define DMA12CS_REQSTOP     (1<<5)                      // DREQ stops DMA
#define DMA12CS_PAUSE       (1<<4)                      // DMA is paused
#define DMA12CS_DREQ        (1<<3)                      // 1=requesting data
#define DMA12CS_INT         (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA12CS_END         (1<<1)                      // 1 when complete
#define DMA12CS_ACTIVE      (1<<0)                      // active

#define SH_DMA12CSPANIC(x)  (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA12CSPTY(x)    (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA12_CBAD          (DMA12_BASE+4)              // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA12CBAD_SCB       (0xffffffff)                // Source Control Block address


#define DMA12_TI            (DMA12_BASE+8)              // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA12TI_WAITS       (0x1f<<21)                  // Add wait cycles
#define DMA12TI_PMAP        (0x1f<<16)                  // peripheral map
#define DMA12TI_BLEN        (0xf<<12)                   // burst transfer length
#define DMA12TI_SRCIGN      (1<<11)                     // Ignore Reads
#define DMA12TI_SRCDREQ     (1<<10)                     // Control source reads with DREQ
#define DMA12TI_SRCWID      (1<<9)                      // Source Transfer Width
#define DMA12TI_SRCINC      (1<<8)                      // Source Address Increment
#define DMA12TI_DSTIGN      (1<<7)                      // Ignore Writes
#define DMA12TI_DSTDREQ     (1<<6)                      // Control source writes with DREQ
#define DMA12TI_DSTWID      (1<<5)                      // Dest Transfer Width
#define DMA12TI_DSTINC      (1<<4)                      // Dest Address Increment
#define DMA12TI_WRESP       (1<<3)                      // Wait for Response
#define DMA12TI_TMODE       (1<<1)                      // 2D Mode
#define DMA12TI_INTEN       (1<<0)                      // Interrupt Enable

#define SH_DMA12TIWAITS(x)  (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA12TIPMAP(x)   (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA12_SRCAD         (DMA12_BASE+0xc)            // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA12SRCAD_SRC      (0xffffffff)                // DMA Source address


#define DMA12_DSTAD         (DMA12_BASE+0x10)           // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA12DSTAD_SRC      (0xffffffff)                // DMA Dest address


#define DMA12_XLEN          (DMA12_BASE+0x14)           // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA12XLEN_X         (0xffff)                    // Transfer bytes length


#define DMA12_STRIDE        (DMA12_BASE+0x18)           // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA12STRIDE_D       (0xffff<<16)                // Destination Stride
#define DMA12STRIDE_S       (0xffff)                    // Source Stride

#define SH_DMA12STRIDED(x)  (((x)&0xffff)<<16)          // shift to the correct position


#define DMA12_NXTCB         (DMA12_BASE+0x1c)           // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA12NXTCB_ADD      (0xffffffff)                // NExt Control Block Address


#define DMA12_DEBUG         (DMA12_BASE+0x20)           // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA12DEBUG_LITE     (1<<28)                     // DMA Lite
#define DMA12DEBUG_VERS     (7<<25)                     // DMA Version
#define DMA12DEBUG_STATE    (0x1ff<<16)                 // DMA State
#define DMA12DEBUG_ID       (0xff<<8)                   // DMA ID
#define DMA12DEBUG_OWRITE   (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA12DEBUG_RERROR   (1<<2)                      // Slave read response error
#define DMA12DEBUG_FERROR   (1<<1)                      // FIFO error
#define DMA12DEBUG_LAST     (1<<0)                      // Read Last Not Set Error


//
// -- The DMA13 controller
//    --------------------
#define DMA13_BASE          (DMA12_BASE+0x100)          // Base Address of DMA13
//-------------------------------------------------------------------------------------------------------------------


#define DMA13_CS            (DMA13_BASE)                // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA13CS_RESET       (1<<31)                     // reset DMA
#define DMA13CS_ABORT       (1<<30)                     // abort current CB
#define DMA13CS_DISDBG      (1<<29)                     // disable debug
#define DMA13CS_WAIT        (1<<28)                     // wait for outstanding writes
#define DMA13CS_PANICPTY    (0xf<<20)                   // priority of panicking transactions
#define DMA13CS_PTY         (0xf<<16)                   // priority of transactions
#define DMA13CS_ERROR       (1<<8)                      // Channel has error
#define DMA13CS_WAITING     (1<<6)                      // is waiting for outstanding writes
#define DMA13CS_REQSTOP     (1<<5)                      // DREQ stops DMA
#define DMA13CS_PAUSE       (1<<4)                      // DMA is paused
#define DMA13CS_DREQ        (1<<3)                      // 1=requesting data
#define DMA13CS_INT         (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA13CS_END         (1<<1)                      // 1 when complete
#define DMA13CS_ACTIVE      (1<<0)                      // active

#define SH_DMA13CSPANIC(x)  (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA13CSPTY(x)    (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA13_CBAD          (DMA13_BASE+4)              // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA13CBAD_SCB       (0xffffffff)                // Source Control Block address


#define DMA13_TI            (DMA13_BASE+8)              // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA13TI_WAITS       (0x1f<<21)                  // Add wait cycles
#define DMA13TI_PMAP        (0x1f<<16)                  // peripheral map
#define DMA13TI_BLEN        (0xf<<12)                   // burst transfer length
#define DMA13TI_SRCIGN      (1<<11)                     // Ignore Reads
#define DMA13TI_SRCDREQ     (1<<10)                     // Control source reads with DREQ
#define DMA13TI_SRCWID      (1<<9)                      // Source Transfer Width
#define DMA13TI_SRCINC      (1<<8)                      // Source Address Increment
#define DMA13TI_DSTIGN      (1<<7)                      // Ignore Writes
#define DMA13TI_DSTDREQ     (1<<6)                      // Control source writes with DREQ
#define DMA13TI_DSTWID      (1<<5)                      // Dest Transfer Width
#define DMA13TI_DSTINC      (1<<4)                      // Dest Address Increment
#define DMA13TI_WRESP       (1<<3)                      // Wait for Response
#define DMA13TI_TMODE       (1<<1)                      // 2D Mode
#define DMA13TI_INTEN       (1<<0)                      // Interrupt Enable

#define SH_DMA13TIWAITS(x)  (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA13TIPMAP(x)   (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA13_SRCAD         (DMA13_BASE+0xc)            // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA13SRCAD_SRC      (0xffffffff)                // DMA Source address


#define DMA13_DSTAD         (DMA13_BASE+0x10)           // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA13DSTAD_SRC      (0xffffffff)                // DMA Dest address


#define DMA13_XLEN          (DMA13_BASE+0x14)           // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA13XLEN_X         (0xffff)                    // Transfer bytes length


#define DMA13_STRIDE        (DMA13_BASE+0x18)           // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA13STRIDE_D       (0xffff<<16)                // Destination Stride
#define DMA13STRIDE_S       (0xffff)                    // Source Stride

#define SH_DMA13STRIDED(x)  (((x)&0xffff)<<16)          // shift to the correct position


#define DMA13_NXTCB         (DMA13_BASE+0x1c)           // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA13NXTCB_ADD      (0xffffffff)                // NExt Control Block Address


#define DMA13_DEBUG         (DMA13_BASE+0x20)           // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA13DEBUG_LITE     (1<<28)                     // DMA Lite
#define DMA13DEBUG_VERS     (7<<25)                     // DMA Version
#define DMA13DEBUG_STATE    (0x1ff<<16)                 // DMA State
#define DMA13DEBUG_ID       (0xff<<8)                   // DMA ID
#define DMA13DEBUG_OWRITE   (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA13DEBUG_RERROR   (1<<2)                      // Slave read response error
#define DMA13DEBUG_FERROR   (1<<1)                      // FIFO error
#define DMA13DEBUG_LAST     (1<<0)                      // Read Last Not Set Error


//
// -- The DMA14 controller
//    --------------------
#define DMA14_BASE          (DMA13_BASE+0x100)          // Base Address of DMA14
//-------------------------------------------------------------------------------------------------------------------


#define DMA14_CS            (DMA14_BASE)                // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA14CS_RESET       (1<<31)                     // reset DMA
#define DMA14CS_ABORT       (1<<30)                     // abort current CB
#define DMA14CS_DISDBG      (1<<29)                     // disable debug
#define DMA14CS_WAIT        (1<<28)                     // wait for outstanding writes
#define DMA14CS_PANICPTY    (0xf<<20)                   // priority of panicking transactions
#define DMA14CS_PTY         (0xf<<16)                   // priority of transactions
#define DMA14CS_ERROR       (1<<8)                      // Channel has error
#define DMA14CS_WAITING     (1<<6)                      // is waiting for outstanding writes
#define DMA14CS_REQSTOP     (1<<5)                      // DREQ stops DMA
#define DMA14CS_PAUSE       (1<<4)                      // DMA is paused
#define DMA14CS_DREQ        (1<<3)                      // 1=requesting data
#define DMA14CS_INT         (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA14CS_END         (1<<1)                      // 1 when complete
#define DMA14CS_ACTIVE      (1<<0)                      // active

#define SH_DMA14CSPANIC(x)  (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA14CSPTY(x)    (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA14_CBAD          (DMA14_BASE+4)              // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA14CBAD_SCB       (0xffffffff)                // Source Control Block address


#define DMA14_TI            (DMA14_BASE+8)              // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA14TI_WAITS       (0x1f<<21)                  // Add wait cycles
#define DMA14TI_PMAP        (0x1f<<16)                  // peripheral map
#define DMA14TI_BLEN        (0xf<<12)                   // burst transfer length
#define DMA14TI_SRCIGN      (1<<11)                     // Ignore Reads
#define DMA14TI_SRCDREQ     (1<<10)                     // Control source reads with DREQ
#define DMA14TI_SRCWID      (1<<9)                      // Source Transfer Width
#define DMA14TI_SRCINC      (1<<8)                      // Source Address Increment
#define DMA14TI_DSTIGN      (1<<7)                      // Ignore Writes
#define DMA14TI_DSTDREQ     (1<<6)                      // Control source writes with DREQ
#define DMA14TI_DSTWID      (1<<5)                      // Dest Transfer Width
#define DMA14TI_DSTINC      (1<<4)                      // Dest Address Increment
#define DMA14TI_WRESP       (1<<3)                      // Wait for Response
#define DMA14TI_TMODE       (1<<1)                      // 2D Mode
#define DMA14TI_INTEN       (1<<0)                      // Interrupt Enable

#define SH_DMA14TIWAITS(x)  (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA14TIPMAP(x)   (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA14_SRCAD         (DMA14_BASE+0xc)            // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA14SRCAD_SRC      (0xffffffff)                // DMA Source address


#define DMA14_DSTAD         (DMA14_BASE+0x10)           // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA14DSTAD_SRC      (0xffffffff)                // DMA Dest address


#define DMA14_XLEN          (DMA14_BASE+0x14)           // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA14XLEN_X         (0xffff)                    // Transfer bytes length


#define DMA14_STRIDE        (DMA14_BASE+0x18)           // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA14STRIDE_D       (0xffff<<16)                // Destination Stride
#define DMA14STRIDE_S       (0xffff)                    // Source Stride

#define SH_DMA14STRIDED(x)  (((x)&0xffff)<<16)          // shift to the correct position


#define DMA14_NXTCB         (DMA14_BASE+0x1c)           // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA14NXTCB_ADD      (0xffffffff)                // NExt Control Block Address


#define DMA14_DEBUG         (DMA14_BASE+0x20)           // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA14DEBUG_LITE     (1<<28)                     // DMA Lite
#define DMA14DEBUG_VERS     (7<<25)                     // DMA Version
#define DMA14DEBUG_STATE    (0x1ff<<16)                 // DMA State
#define DMA14DEBUG_ID       (0xff<<8)                   // DMA ID
#define DMA14DEBUG_OWRITE   (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA14DEBUG_RERROR   (1<<2)                      // Slave read response error
#define DMA14DEBUG_FERROR   (1<<1)                      // FIFO error
#define DMA14DEBUG_LAST     (1<<0)                      // Read Last Not Set Error


//
// -- The DMA15 controller
//    --------------------
#define DMA15_BASE          (HW_BASE+0xe05000)          // Base Address of DMA15
//-------------------------------------------------------------------------------------------------------------------


#define DMA15_CS            (DMA15_BASE)                // Control and Status
//-------------------------------------------------------------------------------------------------------------------
#define DMA15CS_RESET       (1<<31)                     // reset DMA
#define DMA15CS_ABORT       (1<<30)                     // abort current CB
#define DMA15CS_DISDBG      (1<<29)                     // disable debug
#define DMA15CS_WAIT        (1<<28)                     // wait for outstanding writes
#define DMA15CS_PANICPTY    (0xf<<20)                   // priority of panicking transactions
#define DMA15CS_PTY         (0xf<<16)                   // priority of transactions
#define DMA15CS_ERROR       (1<<8)                      // Channel has error
#define DMA15CS_WAITING     (1<<6)                      // is waiting for outstanding writes
#define DMA15CS_REQSTOP     (1<<5)                      // DREQ stops DMA
#define DMA15CS_PAUSE       (1<<4)                      // DMA is paused
#define DMA15CS_DREQ        (1<<3)                      // 1=requesting data
#define DMA15CS_INT         (1<<2)                      // 1=cb ended and INTEN was 1; write 1 to clr
#define DMA15CS_END         (1<<1)                      // 1 when complete
#define DMA15CS_ACTIVE      (1<<0)                      // active

#define SH_DMA15CSPANIC(x)  (((x)&0xf)<<20)             // shift the value to the correct position
#define SH_DMA15CSPTY(x)    (((x)&0xf)<<16)             // shift the value to the correct position


#define DMA15_CBAD          (DMA15_BASE+4)              // Control Block Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA15CBAD_SCB       (0xffffffff)                // Source Control Block address


#define DMA15_TI            (DMA15_BASE+8)              // Transfer Information
//-------------------------------------------------------------------------------------------------------------------
#define DMA15TI_WAITS       (0x1f<<21)                  // Add wait cycles
#define DMA15TI_PMAP        (0x1f<<16)                  // peripheral map
#define DMA15TI_BLEN        (0xf<<12)                   // burst transfer length
#define DMA15TI_SRCIGN      (1<<11)                     // Ignore Reads
#define DMA15TI_SRCDREQ     (1<<10)                     // Control source reads with DREQ
#define DMA15TI_SRCWID      (1<<9)                      // Source Transfer Width
#define DMA15TI_SRCINC      (1<<8)                      // Source Address Increment
#define DMA15TI_DSTIGN      (1<<7)                      // Ignore Writes
#define DMA15TI_DSTDREQ     (1<<6)                      // Control source writes with DREQ
#define DMA15TI_DSTWID      (1<<5)                      // Dest Transfer Width
#define DMA15TI_DSTINC      (1<<4)                      // Dest Address Increment
#define DMA15TI_WRESP       (1<<3)                      // Wait for Response
#define DMA15TI_TMODE       (1<<1)                      // 2D Mode
#define DMA15TI_INTEN       (1<<0)                      // Interrupt Enable

#define SH_DMA15TIWAITS(x)  (((x)&0x1f)<<21)            // shift the value to the correct position
#define SH_DMA15TIPMAP(x)   (((x)&0x1f)<<16)            // shift the value to the correct position


#define DMA15_SRCAD         (DMA15_BASE+0xc)            // Source Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA15SRCAD_SRC      (0xffffffff)                // DMA Source address


#define DMA15_DSTAD         (DMA15_BASE+0x10)           // Destination Address
//-------------------------------------------------------------------------------------------------------------------
#define DMA15DSTAD_SRC      (0xffffffff)                // DMA Dest address


#define DMA15_XLEN          (DMA15_BASE+0x14)           // Transfer Length
//-------------------------------------------------------------------------------------------------------------------
#define DMA15XLEN_X         (0xffff)                    // Transfer bytes length


#define DMA15_STRIDE        (DMA15_BASE+0x18)           // 2D stride
//-------------------------------------------------------------------------------------------------------------------
#define DMA15STRIDE_D       (0xffff<<16)                // Destination Stride
#define DMA15STRIDE_S       (0xffff)                    // Source Stride

#define SH_DMA15STRIDED(x)  (((x)&0xffff)<<16)          // shift to the correct position


#define DMA15_NXTCB         (DMA15_BASE+0x1c)           // Next Control Block
//-------------------------------------------------------------------------------------------------------------------
#define DMA15NXTCB_ADD      (0xffffffff)                // NExt Control Block Address


#define DMA15_DEBUG         (DMA15_BASE+0x20)           // Debug
//-------------------------------------------------------------------------------------------------------------------
#define DMA15DEBUG_LITE     (1<<28)                     // DMA Lite
#define DMA15DEBUG_VERS     (7<<25)                     // DMA Version
#define DMA15DEBUG_STATE    (0x1ff<<16)                 // DMA State
#define DMA15DEBUG_ID       (0xff<<8)                   // DMA ID
#define DMA15DEBUG_OWRITE   (0xf<<4)                    // DMA Outstanding Writes Counter
#define DMA15DEBUG_RERROR   (1<<2)                      // Slave read response error
#define DMA15DEBUG_FERROR   (1<<1)                      // FIFO error
#define DMA15DEBUG_LAST     (1<<0)                      // Read Last Not Set Error


//
// -- The DMA INT_STATUS register
//    ---------------------------
#define DMA_INTSTAT         (DMA0_BASE+0xfe0)           // Interrupt Status Register
//-------------------------------------------------------------------------------------------------------------------
#define DMAINT_15           (1<<15)                     // Interrupt for DMA 15
#define DMAINT_14           (1<<14)                     // Interrupt for DMA 14
#define DMAINT_13           (1<<13)                     // Interrupt for DMA 13
#define DMAINT_12           (1<<12)                     // Interrupt for DMA 12
#define DMAINT_11           (1<<11)                     // Interrupt for DMA 11
#define DMAINT_10           (1<<10)                     // Interrupt for DMA 10
#define DMAINT_9            (1<<9)                      // Interrupt for DMA 9
#define DMAINT_8            (1<<8)                      // Interrupt for DMA 8
#define DMAINT_7            (1<<7)                      // Interrupt for DMA 7
#define DMAINT_6            (1<<6)                      // Interrupt for DMA 6
#define DMAINT_5            (1<<5)                      // Interrupt for DMA 5
#define DMAINT_4            (1<<4)                      // Interrupt for DMA 4
#define DMAINT_3            (1<<3)                      // Interrupt for DMA 3
#define DMAINT_2            (1<<2)                      // Interrupt for DMA 2
#define DMAINT_1            (1<<1)                      // Interrupt for DMA 1
#define DMAINT_0            (1<<0)                      // Interrupt for DMA 0


//
// -- The DMA ENABLE register
//    -----------------------
#define DMA_ENABLE          (DMA0_BASE+0xff0)           // DMA Enable Register
//-------------------------------------------------------------------------------------------------------------------
#define DMAENB_14           (1<<14)                     // Enable DMA 14
#define DMAENB_13           (1<<13)                     // Enable DMA 13
#define DMAENB_12           (1<<12)                     // Enable DMA 12
#define DMAENB_11           (1<<11)                     // Enable DMA 11
#define DMAENB_10           (1<<10)                     // Enable DMA 10
#define DMAENB_9            (1<<9)                      // Enable DMA 9
#define DMAENB_8            (1<<8)                      // Enable DMA 8
#define DMAENB_7            (1<<7)                      // Enable DMA 7
#define DMAENB_6            (1<<6)                      // Enable DMA 6
#define DMAENB_5            (1<<5)                      // Enable DMA 5
#define DMAENB_4            (1<<4)                      // Enable DMA 4
#define DMAENB_3            (1<<3)                      // Enable DMA 3
#define DMAENB_2            (1<<2)                      // Enable DMA 2
#define DMAENB_1            (1<<1)                      // Enable DMA 1
#define DMAENB_0            (1<<0)                      // Enable DMA 0
