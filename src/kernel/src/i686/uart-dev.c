//===================================================================================================================
//
//  kernel i686/uart-dev.c -- This is the UART Device for the x86 32-bit system
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
//  2017-04-09  Initial   0.0.0   ADCL  Initial version
//
//===================================================================================================================


#include "proto.h"
#include "hw.h"
#include "device.h"
#include "proto-x86.h"


//-------------------------------------------------------------------------------------------------------------------
// UartInit() -- initialize the uart1 device structure and prepare the uart to be used
//
// TODO: This initialization is resetting all the GPIO pins; maybe that needs to be moved to another routine.
//-------------------------------------------------------------------------------------------------------------------
static void UartInit(struct serial_device *dev, uint32_t ifc)
{
    //
    // -- Set the fields in the structures
    //    --------------------------------
    dev->hwInterface = ifc;
    dev->dev.valid = true;

    // 
    // -- Disable the UART
    //    ----------------
    dev->ops->close(dev);

    //    
    // -- Mask all interrupts
    //    -------------------
    WriteIoPort(dev->hwInterface + COM_IER, 0x00);

    //
    // -- Enable the FIFO queues
    //    ----------------------
    WriteIoPort(dev->hwInterface + COM_IIR, COMIIR_FIFOTX | COMIIR_FIFORX);
    WriteIoPort(dev->hwInterface + COM_FCR, COMFCR_FIFOEN);
}


//-------------------------------------------------------------------------------------------------------------------
// UartOpen() -- Open a UART for usage
//-------------------------------------------------------------------------------------------------------------------
static void UartOpen(struct serial_device *dev, uint32_t baud, uint32_t bits, char parity, uint32_t stop)
{
    if (!dev || !dev->dev.valid) return;

    //
    // -- Set the parameters for the uart
    //    -------------------------------
    if (dev->ops->chgConfig) dev->ops->chgConfig(dev, baud, bits, parity, stop);
}


//-------------------------------------------------------------------------------------------------------------------
// UartClose() -- Close a UART after using it
//-------------------------------------------------------------------------------------------------------------------
static void UartClose(struct serial_device *dev)
{
    if (!dev || !dev->dev.valid) return;
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
static void UartConfig(struct serial_device *dev, uint32_t baud, uint32_t bits, char parity, uint32_t stop)
{
    if (!dev || !dev->dev.valid) return;

    uint32_t divisor;
    uint8_t baudMsb;
    uint8_t baudLsb;
    uint8_t lcr = 0;

    //
    // -- Determine the stop bits
    //    -----------------------
    if (stop == 2) lcr |= COMLCR_STOP;

    //
    // -- Determine the data width size
    //    -----------------------------
    switch (bits) {
    case 5:
        lcr |= SH_COMLCRWORD_5;
        break;

    case 6:
        lcr |= SH_COMLCRWORD_6;
        break;

    case 7:
        lcr |= SH_COMLCRWORD_7;
        break;

    default:
        lcr |= SH_COMLCRWORD_8;
        break;
    }

    //
    // -- Determine the parity
    //    --------------------
    switch (parity) {
    case 'E':
        lcr |= COMLCR_EVENPAR;
        // -- fall through!!
        
    case 'O':
        lcr |= COMLCR_PAREN;
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
        divisor = 0x180;
        break;

    case 600:
        divisor = 0xc0;
        break;

    case 1200:
        divisor = 0x60;
        break;

    case 2400:
        divisor = 0x3A;
        break;
    
    case 4800:
        divisor = 0x18;
        break;
    
    case 9600:
        divisor = 0xc;
        break;

    case 14400:
        divisor = 0x8;
        break;
    
    case 19200:
        divisor = 0x6;
        break;
    
    case 28800:
        divisor = 0x4;
        break;
    
    case 38400:
        divisor = 0x3;
        break;
    
    case 57600:
        divisor = 0x2;
        break;
    
    case 115200:
    default:
        divisor = 0x1;
        break;
    }

    baudMsb = (divisor >> 8) & 0xff;
    baudLsb = divisor & 0xff;

    //
    // -- Finally, configure the UART
    //    ---------------------------
    WriteIoPort(dev->hwInterface + COM_LCR, COMLCR_DLAB);
    WriteIoPort(dev->hwInterface + COM_DLMSB, baudMsb);
    WriteIoPort(dev->hwInterface + COM_DLLSB, baudLsb);
    WriteIoPort(dev->hwInterface + COM_LCR, lcr);
}


//-------------------------------------------------------------------------------------------------------------------
// UartRxReady() -- Is the Receive FIFO full?
//-------------------------------------------------------------------------------------------------------------------
static bool UartRxReady(struct serial_device *dev)
{
     if (!dev || !dev->dev.valid) return false;

     //
     // -- Read the Flag register and check the status
     //    -------------------------------------------
     return ((ReadIoPort(dev->hwInterface + COM_LSR) & COMLSR_RBF) != 0);
}


//-------------------------------------------------------------------------------------------------------------------
// UartTxEmpty() -- Is the Send FIFO empty?
//-------------------------------------------------------------------------------------------------------------------
static bool UartTxEmpty(struct serial_device *dev)
{
     if (!dev || !dev->dev.valid) return true;

     //
     // -- Read the Flag register and check the status
     //    -------------------------------------------
     return ((ReadIoPort(dev->hwInterface + COM_LSR) & COMLSR_TEMT) != 0);
}


//-------------------------------------------------------------------------------------------------------------------
// UartTxRoom() -- Is the Receive FIFO full?
//-------------------------------------------------------------------------------------------------------------------
static bool UartTxRoom(struct serial_device *dev)
{
     if (!dev || !dev->dev.valid) return false;

     //
     // -- Read the Flag register and check the status
     //    -------------------------------------------
     return ((ReadIoPort(dev->hwInterface + COM_LSR) & COMLSR_THRE) != 0);
}


//-------------------------------------------------------------------------------------------------------------------
// UartRx() -- Read a single character to the UART.
//
// Note that this function busy loops waiting for a character to be ready
//-------------------------------------------------------------------------------------------------------------------
static uint8_t UartRx(struct serial_device *dev)
{
    if (!dev || !dev->dev.valid) return 0xff;

    while (!UartRxReady(dev)) { }
    return ReadIoPort(dev->hwInterface + COM_RBR);
}


//-------------------------------------------------------------------------------------------------------------------
// UartTx() -- Write a single character to the UART.
//
// Note that this function busy loops waiting for the FIFO to have room.
//-------------------------------------------------------------------------------------------------------------------
static void UartTx(struct serial_device *dev, uint8_t byte)
{
    if (!dev || !dev->dev.valid) return;

    while (!UartTxRoom(dev)) { }
    WriteIoPort(dev->hwInterface + COM_THR, byte);
}


//-------------------------------------------------------------------------------------------------------------------
// This structure is the list of functions that are available to operate on the UART1 device
//-------------------------------------------------------------------------------------------------------------------
const struct serial_ops serialOps = {
    .init = UartInit,
    .open = UartOpen,
    .close = UartClose,
    .chgConfig = UartConfig,
    .rxFull = NULL,
    .rxReady = UartRxReady,
    .rxStart = NULL,
    .rxStop = NULL,
    .txEmpty = UartTxEmpty,
    .txRoom = UartTxRoom,
    .txStart = NULL,
    .txStop = NULL,
    .rx = UartRx,
    .tx = UartTx,
};


//-------------------------------------------------------------------------------------------------------------------
// This structure is the abstract definition of the UART1 device
//-------------------------------------------------------------------------------------------------------------------
struct serial_device uart1 = {
    .dev.parent = NULL,
    .dev.devName = "uart1",
    .dev.devClass = DEV_SERIAL,
    .dev.valid = false,             // assume it does not exist
    .hwInterface = COM1,
    .ops = &serialOps,
};


//-------------------------------------------------------------------------------------------------------------------
// UartDevInit() -- Called by early init to initialize the UART for debug messages
//-------------------------------------------------------------------------------------------------------------------
void UartDevInit(void)
{
    uart1.ops->init(&uart1, COM1);
}


//
// -- TODO: These are better suited to be relocated to the serial Driver when I get there.
//    ------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------
// UartPutC() -- Write a single character to the UART.
//
// Note that this function busy loops waiting for the FIFO to have room.
//-------------------------------------------------------------------------------------------------------------------
void UartPutC(const unsigned char byte)
{
    uart1.ops->tx(&uart1, byte);
}


//-------------------------------------------------------------------------------------------------------------------
// UartGetC() -- Read a single character to the UART.
//
// Note that this function busy loops waiting for a character to be ready
//-------------------------------------------------------------------------------------------------------------------
unsigned char UartGetC(void)
{
    return uart1.ops->rx(&uart1);
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
