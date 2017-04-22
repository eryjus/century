//===================================================================================================================
//
//  loader framebuffer-common.c -- Framebuffer functions common across all architectures
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


#include "hw.h"
#include "frame-buffer.h"
#include "proto.h"


//
// -- An internal variable for the current color to print
//    ---------------------------------------------------
static uint16_t color = 0xffff;
static uint16_t bgColor = 0x0000;
static uint32_t row = 0;
static uint32_t col = 0;


//
// -- This is the internal common implementation of the framebuffer
//    -------------------------------------------------------------
struct FrameBufferInfo frameBufferInfo;


//-------------------------------------------------------------------------------------------------------------------
// FrameBufferClear() -- Clear the screen defined by the frame buffer
//-------------------------------------------------------------------------------------------------------------------
void FrameBufferClear(void)
{
    //
    // -- calculate the number of 16-bit words to write (rows * cols)
    //    -----------------------------------------------------------------------
    int cnt = frameBufferInfo.height * frameBufferInfo.width;
    uint16_t *b = frameBufferInfo.buffer;

    for (int i = 0; i < cnt; i ++) {
        b[i] = bgColor;
    }

    row = 0;
    col = 0;
}


//-------------------------------------------------------------------------------------------------------------------
// FrameBufferDrawChar() -- Draw a character on the screen
//-------------------------------------------------------------------------------------------------------------------
void FrameBufferDrawChar(int ch)
{
    if (!frameBufferInfo.buffer) return;

    if (ch == '\n') {
        col = 0;
        row ++;
        return;
    }

    if (ch == '\t') {
        col += (8 - (col % 8));
        
        if (col > frameBufferInfo.width / 8) {
            col = 0;
            row ++;
        }

        return;
    }

    uint8_t *chImg = &systemFont[ch * 16];              // first the character image (16 rows per image)
    uint16_t *where = &frameBufferInfo.buffer[(row * 16 * frameBufferInfo.width) + (col * 8)];

    for (int i = 0; i < 16; i ++, where += frameBufferInfo.width) {
        uint8_t c = chImg[i];

        for (int j = 0; j < 8; j ++, c = c >> 1) {
            if (c & 0x01) where[j] = color;
            else where[j] = bgColor;
        }
    }  

    col ++;
}


//-------------------------------------------------------------------------------------------------------------------
// FrameBufferSetColor() -- set the color code (uint16_t) to be the result of parsing the string ("#FFFFFF")
//-------------------------------------------------------------------------------------------------------------------
uint16_t FrameBufferSetColor(const char *c)
{
    int r;
    int g;
    int b;

    if (!c || strlen(c) != 7 || *c != '#') return 0;

    // -- OK, we have a well formatted string, compute each character in turn: char 1
    if (c[1] >= '0' && c[1] <= '9') r = c[1] - '0';
    else if (c[1] >= 'a' && c[1] <= 'f') r = c[1] - 'a' + 10;
    else if (c[1] >= 'A' && c[1] <= 'F') r = c[1] - 'A' + 10;
    else return 0;

    // -- char 2
    r *= 16;
    if (c[2] >= '0' && c[2] <= '9') r = r + c[2] - '0';
    else if (c[2] >= 'a' && c[2] <= 'f') r = r + c[2] - 'a' + 10;
    else if (c[2] >= 'A' && c[2] <= 'F') r = r + c[2] - 'A' + 10;
    else return 0;

    // -- char 3
    if (c[3] >= '0' && c[3] <= '9') g = c[3] - '0';
    else if (c[3] >= 'a' && c[3] <= 'f') g = c[3] - 'a' + 10;
    else if (c[3] >= 'A' && c[3] <= 'F') g = c[3] - 'A' + 10;
    else return 0;

    // -- char 4
    g *= 16;
    if (c[4] >= '0' && c[4] <= '9') g = g + c[4] - '0';
    else if (c[4] >= 'a' && c[4] <= 'f') g = g + c[4] - 'a' + 10;
    else if (c[4] >= 'A' && c[4] <= 'F') g = g + c[4] - 'A' + 10;
    else return 0;

    // -- char 5
    if (c[5] >= '0' && c[5] <= '9') b = c[5] - '0';
    else if (c[5] >= 'a' && c[5] <= 'f') b = c[5] - 'a' + 10;
    else if (c[5] >= 'A' && c[5] <= 'F') b = c[5] - 'A' + 10;
    else return 0;

    // -- char 6
    b *= 16;
    if (c[6] >= '0' && c[6] <= '9') b = b + c[6] - '0';
    else if (c[6] >= 'a' && c[6] <= 'f') b = b + c[6] - 'a' + 10;
    else if (c[6] >= 'A' && c[6] <= 'F') b = b + c[6] - 'A' + 10;
    else return 0;

    // -- OK, each of r, g, and b have a value from 0 to 255 (0x00 to 0xff, or 0b00000000 to 0b11111111)
    //    shift those down to the proper size for the standard.
    r = (r & 0xff) >> 3;
    g = (g & 0xff) >> 2;
    b = (b & 0xff) >> 3;

    color = (r << 11) | (g << 5) | b;
    return color;
}


//-------------------------------------------------------------------------------------------------------------------
// putc() -- put a character to a stream 
//
//  Note that with our loader the only stream we will output to is stdout.  Therefore, there is no need for the 
//  stream parameter which is ignored.
//-------------------------------------------------------------------------------------------------------------------
int putc(int ch, FILE *f)
{
    extern void UartPutC(const unsigned char byte);

    UartPutC(ch);
    FrameBufferDrawChar(ch);
    return ch;
}