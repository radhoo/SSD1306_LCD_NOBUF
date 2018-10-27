/*
** SSD1306 I2C 128x64 Monochrome OLED LCD Module display Library
** Copyright (C) 2009 - 2018 Radu Motisan, radu.motisan@gmail.com, www.pocketmagic.net
**
** This file is a part of "SSD1306 NO BUFFER" open source library.
**
** ILI9341 Library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Arduino.h"
#include <Wire.h>

// define I2C address
#define SSD1306_ADDRESS   0x3C  

// define size
#define SSD1306_WIDTH	128
#define SSD1306_HEIGHT	64

// define colors
#define BLACK 0
#define WHITE 1
#define INVERSE 2
#define TRANSPARENT 0xff

// define level 1 commands
#define SSD1306_CMD_SETCONTRAST 0x81
#define SSD1306_CMD_DISPLAYALLON_RESUME 0xA4
#define SSD1306_CMD_DISPLAYALLON 0xA5
#define SSD1306_CMD_NORMALDISPLAY 0xA6
#define SSD1306_CMD_INVERTDISPLAY 0xA7
#define SSD1306_CMD_DISPLAYOFF 0xAE
#define SSD1306_CMD_DISPLAYON 0xAF

#define SSD1306_CMD_SETDISPLAYOFFSET 0xD3
#define SSD1306_CMD_SETCOMPINS 0xDA

#define SSD1306_CMD_SETVCOMDETECT 0xDB

#define SSD1306_CMD_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_CMD_SETPRECHARGE 0xD9

#define SSD1306_CMD_SETMULTIPLEX 0xA8

#define SSD1306_CMD_SETLOWCOLUMN 0x00
#define SSD1306_CMD_SETHIGHCOLUMN 0x10

#define SSD1306_CMD_SETSTARTLINE 0x40

#define SSD1306_CMD_MEMORYMODE 0x20
#define SSD1306_CMD_COLUMNADDR 0x21
#define SSD1306_CMD_PAGEADDR   0x22

#define SSD1306_CMD_COMSCANINC 0xC0
#define SSD1306_CMD_COMSCANDEC 0xC8

#define SSD1306_CMD_SEGREMAP 0xA0

#define SSD1306_CMD_CHARGEPUMP 0x8D

#define SSD1306_CMD_EXTERNALVCC 0x1
#define SSD1306_CMD_SWITCHCAPVCC 0x2

// level 2
#define SSD1306_CMD_ACTIVATE_SCROLL 0x2F
#define SSD1306_CMD_DEACTIVATE_SCROLL 0x2E


class ssd1306_128x64_i2c {
	uint8_t m_svcc;
	uint8_t m_i2caddr;
public:
	void init(uint8_t switchvcc = SSD1306_CMD_SWITCHCAPVCC, uint8_t i2caddr = SSD1306_ADDRESS);
	void setPos(uint8_t x, uint8_t y);
	void clearDisplay();
	void writeCommand(uint8_t c);
	void invertDisplay(uint8_t i);

	
  	void contrast(uint8_t contrast);
  	void drawPixel(int8_t x, int8_t y, uint8_t color);
  	void drawCircle(int8_t x, int8_t y, int8_t radius, int8_t color);
  	void drawChar(int8_t x, int8_t row, char c);				// one row is 8 pixels wide
  	void drawString(uint8_t x, uint8_t row, const char *string);// one row is 8 pixels wide
};

