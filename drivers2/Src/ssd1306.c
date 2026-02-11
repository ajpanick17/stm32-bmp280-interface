/*
 * SSD1306.c
 *
 *  Created on: Oct 13, 2025
 */

// ssd1306_simple.c
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "stm32f407xx_i2c_driver.h"

// ------------ CONFIG ------------
#define OLED_WIDTH   128
#define OLED_HEIGHT   64

extern I2C_Handle_t I2C1handle;

// FrameBuffer: 128 x 64 / 8 = 1024 bytes
static uint8_t FrameBuffer[OLED_WIDTH * (OLED_HEIGHT / 8)];

static const uint8_t FONT[][5] = {
    // 0: space
    {0x00,0x00,0x00,0x00,0x00},      // ' '
    {0x3E,0x51,0x49,0x45,0x3E},      // '0'
    {0x00,0x42,0x7F,0x40,0x00},      // '1'
    {0x62,0x51,0x49,0x49,0x46},      // '2'
    {0x22,0x41,0x49,0x49,0x36},      // '3'
    {0x18,0x14,0x12,0x7F,0x10},      // '4'
    {0x2F,0x49,0x49,0x49,0x31},      // '5'
    {0x3E,0x49,0x49,0x49,0x30},      // '6'
    {0x01,0x71,0x09,0x05,0x03},      // '7'
    {0x36,0x49,0x49,0x49,0x36},      // '8'
    {0x06,0x49,0x49,0x29,0x1E},      // '9'
    {0x01,0x01,0x7F,0x01,0x01},      // 'T'
    {0x7F,0x49,0x49,0x49,0x41},      // 'E'
    {0x7F,0x02,0x0C,0x02,0x7F},      // 'M'
    {0x7F,0x09,0x09,0x09,0x06},      // 'P'
    {0x7F,0x41,0x41,0x22,0x1C},      // 'R'
    {0x26,0x49,0x49,0x49,0x32},      // 'S'
    {0x14,0x14,0x14,0x14,0x14},      // '='
    {0x00,0x00,0x60,0x60,0x00},      // '.'
    {0x63,0x13,0x08,0x64,0x63},      // '%'
    {0x08,0x08,0x08,0x08,0x08},      // '-'
};

#define FONT_COUNT  (sizeof(FONT)/sizeof(FONT[0]))

void draw_char_page(int page, int x, char c)
{
    if (page < 0 || page >= (OLED_HEIGHT/8)) return;
    if (x < 0 || x > (OLED_WIDTH - 5)) return;

    int index = 0; // default: space

    if (c == ' ') {
        index = 0;
    }
    else if (c >= '0' && c <= '9') {
        index = 1 + (c - '0');    // '0' -> 1, '9' -> 10
    }
    else if (c == 'T') index = 11;
    else if (c == 'E') index = 12;
    else if (c == 'M') index = 13;
    else if (c == 'P') index = 14;
    else if (c == 'R') index = 15;
    else if (c == 'S') index = 16;
    else if (c == '=') index = 17;
    else if (c == '.') index = 18;
    else if (c == '%') index = 19;
    else if (c == '-') index = 20;
    else index = 0; // anything else => space

    if (index < 0 || index >= (int)FONT_COUNT) index = 0;

    const uint8_t *glyph = FONT[index];

    // Copy 5 columns of glyph into FrameBuffer
    for (int col = 0; col < 5; ++col)
    {
        FrameBuffer[page * OLED_WIDTH + (x + col)] = glyph[col];
    }
}

void ssd1306_print(int page, const char *text)
{
    if (page < 0 || page >= (OLED_HEIGHT/8)) return;

    // 1) Clear that page line in FrameBuffer
    for (int i = 0; i < OLED_WIDTH; ++i) {
        FrameBuffer[page * OLED_WIDTH + i] = 0x00;
    }

    // 2) Draw characters
    int x = 0;
    while (*text && x <= (OLED_WIDTH - 6)) {
        draw_char_page(page, x, *text);
        x += 6;   // 5 pixels glyph + 1 pixel spacing
        text++;
    }

    // 3) Send that page to the OLED
    uint8_t cmd[] = {0x21, 0, OLED_WIDTH - 1, 0x22, (uint8_t)page, (uint8_t)page};
    I2C_Burst_Write(&I2C1handle,0x3C,0x00,6,cmd);

    uint8_t data[1 + OLED_WIDTH];
    data[0] = 0x40; // data indicator
    for (int i = 0; i < OLED_WIDTH; ++i) {
        //data[1 + i] = FrameBuffer[page * OLED_WIDTH + i];
    	data[1 + i] = 0xFF;
    }
    I2C_Burst_Write(&I2C1handle,0x3C,0x00,129,data);
}
