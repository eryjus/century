//===================================================================================================================
//
//  kernel i686/uart-dev.c -- This is the UART Device for the i686 32-bit CPU
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
//  2017-04-22  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "types.h"
#include "proto.h"


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
//-------------------------------------------------------------------------------------------------------------------
static void UartInit(void)
{
    // 
    // -- Disable the UART
    //    ----------------
    UartClose();

    // 
    // -- Clear any pending interrupts
    //    ----------------------------
    outb(COM1 + UART_IER, 0x00);

    //
    // -- Enable the FIFO queues
    //    ----------------------
    outb(COM1 + UART_FCR, UART_FIFO | UART_RCVRFIFO | UART_XMITFIFO);

    UartOpen(115200, 8, 'N', 1);
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
    outb(COM1 + UART_MCR, UART_RTS | UART_DTR);
}


//-------------------------------------------------------------------------------------------------------------------
// UartClose() -- Close a UART after using it
//-------------------------------------------------------------------------------------------------------------------
static void UartClose(void)
{
    outb(COM1 + UART_FCR, 0x00);
    outb(COM1 + UART_IER, 0x00);
    outb(COM1 + UART_MCR, 0x00);
    outb(COM1 + UART_LCR, 0x00);
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
//-------------------------------------------------------------------------------------------------------------------
static void UartConfig(uint32_t baud, uint32_t bits, char parity, uint32_t stop)
{
    uint32_t baudHi = 0x00;
    uint32_t baudLo;
    uint32_t lcrh = 0;

    //
    // -- Determine the stop bits
    //    -----------------------
    if (stop == 2) lcrh |= UART_STP;

    //
    // -- Determine the data width size
    //    -----------------------------
    switch (bits) {
    case 5:
        lcrh |= UART_WLS5;
        break;

    case 6:
        lcrh |= UART_WLS6;
        break;

    case 7:
        lcrh |= UART_WLS7;
        break;

    default:
        lcrh |= UART_WLS8;
        break;
    }

    //
    // -- Determine the parity
    //    --------------------
    switch (parity) {
    case 'E':
        lcrh |= UART_EPS;
        // fall through
        
    case 'O':
        lcrh |= UART_PEN;
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
        baudHi = 0x01;
        baudLo = 0x80;
        break;

    case 600:
        baudLo = 0xc0;
        break;

    case 1200:
        baudLo = 0x60;
        break;

    case 2400:
        baudLo = 0x30;
        break;
    
    case 4800:
        baudLo = 0x18;
        break;
    
    case 9600:
        baudLo = 0x0c;
        break;

    case 14400:
        baudLo = 0x08;
        break;
    
    case 19200:
        baudLo = 0x06;
        break;
    
    case 28800:
        baudLo = 0x04;
        break;
    
    case 38400:
        baudLo = 0x03;
        break;
    
    case 57600:
        baudLo = 0x02;
        break;
    
    case 115200:
    default:
        baudLo = 0x01;
        break;
    }

    //
    // -- Finally, configure the UART
    //    ---------------------------
    outb(COM1 + UART_LCR, UART_DLAB);
    outb(COM1 + UART_DLM, baudHi);
    outb(COM1 + UART_DLL, baudLo);
    outb(COM1 + UART_LCR, lcrh);
}


//-------------------------------------------------------------------------------------------------------------------
// UartRxReady() -- Is the Receive FIFO full?
//-------------------------------------------------------------------------------------------------------------------
static bool UartRxReady(void)
{
    //
    // -- Read the Flag register and check the status
    //    -------------------------------------------
    return ((inb(COM1 + UART_LSR) & UART_DR) != 0);
}


//-------------------------------------------------------------------------------------------------------------------
// UartTxRoom() -- Is the Receive FIFO full?
//-------------------------------------------------------------------------------------------------------------------
static bool UartTxRoom(void)
{
    //
    // -- Read the Flag register and check the status
    //    -------------------------------------------
    return ((inb(COM1 + UART_LSR) & UART_THRE) != 0);
}


//-------------------------------------------------------------------------------------------------------------------
// UartRx() -- Read a single character to the UART.
//
// Note that this function busy loops waiting for a character to be ready
//-------------------------------------------------------------------------------------------------------------------
static uint8_t UartRx(void)
{
    while (!UartRxReady()) { }
    return inb(COM1 + UART_RBR);
}


//-------------------------------------------------------------------------------------------------------------------
// UartTx() -- Write a single character to the UART.
//
// Note that this function busy loops waiting for the FIFO to have room.
//-------------------------------------------------------------------------------------------------------------------
static void UartTx(uint8_t byte)
{
    while (!UartTxRoom()) { }
    outb(COM1 + UART_THR, byte);
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
