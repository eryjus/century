//===================================================================================================================
//
//  kernel uart.c -- This is the UART Driver
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


#include "proto.h"
#include "hw.h"


//-------------------------------------------------------------------------------------------------------------------
// UartInit() -- Initialize the UART.  The end result is a UART set up for 115200-N-8-1.  Interrupts will be
//               disabled.
//
// TODO: This initialization is resetting all the GPIO pins; maybe that needs to be moved to another routine.
//-------------------------------------------------------------------------------------------------------------------
void UartInit(void)
{
    // -- Disable the UART
    WriteMmio(UART_CR, 0x00000000); 

    // -- Enable GPIO pins 14/15 only
    WriteMmio(GPIO_GPPUD, 0x00000000);
    BusyWait(150);
    WriteMmio(GPIO_GPPUDCLK1, GPIOCLK1_14 | GPIOCLK1_15);
    BusyWait(150);
    WriteMmio(GPIO_GPPUDCLK1, 0x00000000);              // LEARN: Why does this make sense?

    // -- Clear any pending interrupts
    WriteMmio(UART_ICR, UARTMIS_OEIC | UARTMIS_BEIC | UARTMIS_PEIC | UARTMIS_FEIC | 
            UARTMIS_RTIC | UARTMIS_TXIC | UARTMIS_RXIC | UARTMIS_CTSMIC);

    // -- Set up the UART comm parameters: 115200-N-8-1.  Now:
    //    UART_CLOCK = 3000000
    //    Divider = UART_CLOCK / (16 * 115200) = 1.627 = ~1
    //    Fraction = (0.627 * 64) + 0.5 = 40.6 = ~40
    //    ----------------------------------------------------
    WriteMmio(UART_IBRD, 1);
    WriteMmio(UART_FBRD, 40);
    WriteMmio(UART_LCRH, UARTLCRH_FEN | SH_UARTLCRHWLEN_8);

    // -- Mask all interrupts
    WriteMmio(UART_IMSC, UARTIMSC_OEIM | UARTIMSC_BEIM | UARTIMSC_PEIM | UARTIMSC_FEIM |
            UARTIMSC_RTIM | UARTIMSC_TXIM | UARTIMSC_RXIM | UARTIMSC_CTSMIM);

    // -- Enable the newly configured UART
    WriteMmio(UART_CR, UARTCR_RXE | UARTCR_TXE | UARTCR_EN);
}


//-------------------------------------------------------------------------------------------------------------------
// UartPutC() -- Write a single character to the UART.
//
// Note that this function busy loops waiting for the FIFO to have room.
//-------------------------------------------------------------------------------------------------------------------
void UartPutC(const unsigned char byte)
{
    while (ReadMmio(UART_FR) & UARTFR_TXFE) { }
    WriteMmio(UART_DR, byte);
}


//-------------------------------------------------------------------------------------------------------------------
// UartGetC() -- Read a single character to the UART.
//
// Note that this function busy loops waiting for a character to be ready
//-------------------------------------------------------------------------------------------------------------------
unsigned char UartGetC(void)
{
    while (ReadMmio(UART_FR) & UARTFR_RXFE) { }
    return ReadMmio(UART_DR);
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
