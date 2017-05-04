//===================================================================================================================
//
//  kernel rpi2b/uart-dev.c -- This is the UART Device for the Raspberry Pi 2B
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
//  2017-04-05  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"
#include "mmio.h"


//-------------------------------------------------------------------------------------------------------------------
// Local Prototypes
//-------------------------------------------------------------------------------------------------------------------
void BusyWait(uint32_t microSecs);
static void UartInit(void);
static void UartOpen(uint32_t baud, uint32_t bits, char parity, uint32_t stop);
static void UartClose(void);
static void UartConfig(uint32_t baud, uint32_t bits, char parity, uint32_t stop);
static bool UartRxReady(void);
static bool UartTxRoom(void);
static uint8_t UartRx(void);
static void UartTx(uint8_t byte);


//-------------------------------------------------------------------------------------------------------------------
// UartInit() -- initialize the uart1 device structure and prepare the uart to be used
//
// TODO: This initialization is resetting all the GPIO pins; maybe that needs to be moved to another routine.
//-------------------------------------------------------------------------------------------------------------------
static void UartInit(void)
{
    // 
    // -- Disable the UART
    //    ----------------
    UartClose();

    //
    // -- Enable GPIO pins 14/15 only
    //    ---------------------------
    MmioWrite(GPIO_GPPUD, 0x00000000);
    BusyWait(150);
    MmioWrite(GPIO_GPPUDCLK1, GPIOCLK1_14 | GPIOCLK1_15);
    BusyWait(150);
    MmioWrite(GPIO_GPPUDCLK1, 0x00000000);              // LEARN: Why does this make sense?

    // 
    // -- Clear any pending interrupts
    //    ----------------------------
    MmioWrite(UART_BASE + UART_ICR, UARTMIS_OEIC | UARTMIS_BEIC | UARTMIS_PEIC | UARTMIS_FEIC | 
                                           UARTMIS_RTIC | UARTMIS_TXIC | UARTMIS_RXIC | UARTMIS_CTSMIC);

    //    
    // -- Mask all interrupts
    //    -------------------
    MmioWrite(UART_BASE + UART_IMSC, UARTIMSC_OEIM | UARTIMSC_BEIM | UARTIMSC_PEIM | UARTIMSC_FEIM |
                                            UARTIMSC_RTIM | UARTIMSC_TXIM | UARTIMSC_RXIM | UARTIMSC_CTSMIM);

    //
    // -- Enable the FIFO queues
    //    ----------------------
    MmioWrite(UART_BASE + UART_LCRH, UARTLCRH_FEN);
}


//-------------------------------------------------------------------------------------------------------------------
// UartOpen() -- Open a UART for usage
//-------------------------------------------------------------------------------------------------------------------
static void UartOpen(uint32_t baud, uint32_t bits, char parity, uint32_t stop)
{
    //
    // -- Set the parameters for the uart
    //    -------------------------------
    UartConfig(baud, bits, parity, stop);

    //
    // -- Enable the newly configured UART (not transmitting/receiving yet)
    //    -----------------------------------------------------------------
    MmioWrite(UART_BASE + UART_CR, UARTCR_EN);
}


//-------------------------------------------------------------------------------------------------------------------
// UartClose() -- Close a UART after using it
//-------------------------------------------------------------------------------------------------------------------
static void UartClose(void)
{
    MmioWrite(UART_BASE + UART_CR, 0x00000000); 
}


//-------------------------------------------------------------------------------------------------------------------
// UartConfig() -- Configure the UART for usage
//
// Note, we are not disabling the UART to reconfigure it; we might want to....
//
// This function has a bit to do here...  Baud needs to be broken down into a proper integer and factional part.
// For now, we are only going to support the standard baud rates of: 300, 600, 1200, 2400, 4800, 9600, 14400,
// 19200, 28800, 38400, 57600, or 115200 (default).  This is because we would have to implement at least in part 
// a math library to support more.  Parity needs to be determined and the proper flags set for writing to the UART.  
// Stop bits is either 1 or 2.  Bits can be 5, 6, 7, or 8.
//
// * If stop bits is requested to be 2, we send UARTLCRH_STP2 to UART_LCRH
// * The default stop bits is 1, which does not modify UART_LCRH
// * If word size is 5, SH_UARTLCRHWLEN_5 will be sent to UART_LCRH
// * If word size is 6, SH_UARTLCRHWLEN_6 will be sent to UART_LCRH
// * If word size is 7, SH_UARTLCRHWLEN_7 will be sent to UART_LCRH
// * The default word size is 8, in which SH_UARTLCRHWLEN_8 will be sent to UART_LCRH
// * For Parity = 'O' (Odd), UARTLCRH_PEN will be sent to UART_LCRH
// * For Parity = 'E' (Even), UARTLCRH_PEN and UARTLCRH_EPS will be sent to UART_LCRH
// * The default parity is 'N', which does not modify UART_LCRH
//
// When we get there:
// * The integer part of the baud divisor is: 3000000 / (16 * baud)
// * The fractional part of the baud divisor is: ((((3000000 / (16 * baud)) - (iBaud * 100)) * 64) + 50) / 100
//-------------------------------------------------------------------------------------------------------------------
static void UartConfig(uint32_t baud, uint32_t bits, char parity, uint32_t stop)
{
    uint32_t iBaud;
    uint32_t fBaud;
    uint32_t lcrh = 0;

    //
    // -- Determine the stop bits
    //    -----------------------
    if (stop == 2) lcrh |= UARTLCRH_STP2;

    //
    // -- Determine the data width size
    //    -----------------------------
    switch (bits) {
    case 5:
        lcrh |= SH_UARTLCRHWLEN_5;
        break;

    case 6:
        lcrh |= SH_UARTLCRHWLEN_5;
        break;

    case 7:
        lcrh |= SH_UARTLCRHWLEN_7;
        break;

    default:
        lcrh |= SH_UARTLCRHWLEN_8;
        break;
    }

    //
    // -- Determine the parity
    //    --------------------
    switch (parity) {
    case 'E':
        lcrh |= UARTLCRH_EPS;
        // -- fall through!!
        
    case 'O':
        lcrh |= UARTLCRH_PEN;
        break;

    default:
        break;
    }

    //
    // -- calculate the baud rate integer and fractional divisor parts.  Note this works since it can be calculated
    //    statically at compile time -- even for fBaud since iBaud is constant.
    //    ---------------------------------------------------------------------------------------------------------
    switch (baud) {
    case 300:
        iBaud = 3000000 / (16 * 300);
        fBaud = ((((3000000 / (16 * 300)) - (iBaud * 100)) * 64) + 50) / 100;
        break;

    case 600:
        iBaud = 3000000 / (16 * 600);
        fBaud = ((((3000000 / (16 * 600)) - (iBaud * 100)) * 64) + 50) / 100;
        break;

    case 1200:
        iBaud = 3000000 / (16 * 1200);
        fBaud = ((((3000000 / (16 * 1200)) - (iBaud * 100)) * 64) + 50) / 100;
        break;

    case 2400:
        iBaud = 3000000 / (16 * 2400);
        fBaud = ((((3000000 / (16 * 2400)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    
    case 4800:
        iBaud = 3000000 / (16 * 4800);
        fBaud = ((((3000000 / (16 * 4800)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    
    case 9600:
        iBaud = 3000000 / (16 * 9600);
        fBaud = ((((3000000 / (16 * 9600)) - (iBaud * 100)) * 64) + 50) / 100;
        break;

    case 14400:
        iBaud = 3000000 / (16 * 14400);
        fBaud = ((((3000000 / (16 * 14400)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    
    case 19200:
        iBaud = 3000000 / (16 * 19200);
        fBaud = ((((3000000 / (16 * 19200)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    
    case 28800:
        iBaud = 3000000 / (16 * 28800);
        fBaud = ((((3000000 / (16 * 28800)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    
    case 38400:
        iBaud = 3000000 / (16 * 38400);
        fBaud = ((((3000000 / (16 * 38400)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    
    case 57600:
        iBaud = 3000000 / (16 * 57600);
        fBaud = ((((3000000 / (16 * 57600)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    
    case 115200:
    default:
        iBaud = 3000000 / (16 * 115200);
        fBaud = ((((3000000 / (16 * 115200)) - (iBaud * 100)) * 64) + 50) / 100;
        break;
    }

    //
    // -- Finally, configure the UART
    //    ---------------------------
    MmioWrite(UART_BASE + UART_IBRD, iBaud);
    MmioWrite(UART_BASE + UART_FBRD, fBaud);
    MmioWrite(UART_BASE + UART_LCRH, lcrh);
}


//-------------------------------------------------------------------------------------------------------------------
// UartRxReady() -- Is the Receive FIFO full?
//-------------------------------------------------------------------------------------------------------------------
static bool UartRxReady(void)
{
     //
     // -- Read the Flag register and check the status
     //    -------------------------------------------
     return ((MmioRead(UART_BASE + UART_FR) & UARTFR_RXFF) == 0);
}


//-------------------------------------------------------------------------------------------------------------------
// UartTxRoom() -- Is the Receive FIFO full?
//-------------------------------------------------------------------------------------------------------------------
static bool UartTxRoom(void)
{
     //
     // -- Read the Flag register and check the status
     //    -------------------------------------------
     return ((MmioRead(UART_BASE + UART_FR) & UARTFR_TXFF) == 0);
}


//-------------------------------------------------------------------------------------------------------------------
// UartRx() -- Read a single character to the UART.
//
// Note that this function busy loops waiting for a character to be ready
//-------------------------------------------------------------------------------------------------------------------
static uint8_t UartRx(void)
{
    while (!UartRxReady()) { }
    return MmioRead(UART_BASE + UART_DR);
}


//-------------------------------------------------------------------------------------------------------------------
// UartTx() -- Write a single character to the UART.
//
// Note that this function busy loops waiting for the FIFO to have room.
//-------------------------------------------------------------------------------------------------------------------
static void UartTx(uint8_t byte)
{
    while (!UartTxRoom()) { }
    MmioWrite(UART_BASE + UART_DR, byte);
}


//-------------------------------------------------------------------------------------------------------------------
// UartDevInit() -- Called by early init to initialize the UART for debug messages
//-------------------------------------------------------------------------------------------------------------------
void UartDevInit(void)
{
    UartInit();
    UartOpen(115200, 8, 'N', 1);
}


//
// -- TODO: These are better suited to be relocated to the serial Driver when I get there.
//    ------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------
// UartPutC() -- Write a single character to the UART.
//
// Note that this function busy loops waiting for the FIFO to have room.
//-------------------------------------------------------------------------------------------------------------------
void UartPutC(const utf8_t byte)
{
    UartTx(byte);
}


//-------------------------------------------------------------------------------------------------------------------
// UartGetC() -- Read a single character to the UART.
//
// Note that this function busy loops waiting for a character to be ready
//-------------------------------------------------------------------------------------------------------------------
unsigned char UartGetC(void)
{
    return UartRx();
}


//-------------------------------------------------------------------------------------------------------------------
// UartWriteBuf() -- Write a buffer to the UART
//-------------------------------------------------------------------------------------------------------------------
void UartWriteBuf(const unsigned char *buffer, size_t size)
{
    for (int i = 0; i < size; i ++) UartPutC(buffer[i]);
}


//-------------------------------------------------------------------------------------------------------------------
// UartPutS() -- Write a string to the UART
//-------------------------------------------------------------------------------------------------------------------
void UartPutS(const char *str)
{
    UartWriteBuf((unsigned char *)str, strlen(str));
}
