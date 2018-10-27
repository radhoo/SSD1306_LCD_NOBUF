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

#include "ssd1306_128x64_i2c.h"
#include "font5x8.h"

void ssd1306_128x64_i2c::init(uint8_t switchvcc, uint8_t i2caddr) {
	m_svcc = switchvcc;
	m_i2caddr = i2caddr;

	 // I2C Init
	Wire.setClock(400000);//fast mode
    Wire.begin();

	// Init sequence
	writeCommand(SSD1306_CMD_DISPLAYOFF);                    // 0xAE
	writeCommand(SSD1306_CMD_SETDISPLAYCLOCKDIV);            // 0xD5
	writeCommand(0x80);                                  // the suggested ratio 0x80

	writeCommand(SSD1306_CMD_SETMULTIPLEX);                  // 0xA8
	writeCommand(SSD1306_HEIGHT - 1);

	writeCommand(SSD1306_CMD_SETDISPLAYOFFSET);              // 0xD3
	writeCommand(0x0);                                   // no offset
	writeCommand(SSD1306_CMD_SETSTARTLINE | 0x0);            // line #0
	writeCommand(SSD1306_CMD_CHARGEPUMP);                    // 0x8D
	if (m_svcc == SSD1306_CMD_EXTERNALVCC)
		writeCommand(0x10);
	else
		writeCommand(0x14);
	writeCommand(SSD1306_CMD_MEMORYMODE);                    // 0x20
	writeCommand(0x00);                                  // 0x0 act like ks0108
	writeCommand(SSD1306_CMD_SEGREMAP | 0x1);
	writeCommand(SSD1306_CMD_COMSCANDEC);

	// 128x64
	writeCommand(SSD1306_CMD_SETCOMPINS);                    // 0xDA
	writeCommand(0x12);
	writeCommand(SSD1306_CMD_SETCONTRAST);                   // 0x81
	if (m_svcc == SSD1306_CMD_EXTERNALVCC)
		writeCommand(0x9F); 
	else
		writeCommand(0xCF);

	
	writeCommand(SSD1306_CMD_SETPRECHARGE);                  // 0xd9
	if (m_svcc == SSD1306_CMD_EXTERNALVCC)
		writeCommand(0x22); 
	else
		writeCommand(0xF1); 
	writeCommand(SSD1306_CMD_SETVCOMDETECT);                 // 0xDB
	writeCommand(0x40);
	writeCommand(SSD1306_CMD_DISPLAYALLON_RESUME);           // 0xA4
	writeCommand(SSD1306_CMD_NORMALDISPLAY);                 // 0xA6

	writeCommand(SSD1306_CMD_DEACTIVATE_SCROLL);

	writeCommand(SSD1306_CMD_DISPLAYON);					//--turn on oled panel  	


	// clear 
	clearDisplay();

}

void ssd1306_128x64_i2c::setPos(uint8_t x, uint8_t y) {
	// set column
	writeCommand(SSD1306_CMD_COLUMNADDR);
	writeCommand(x);   // Column start address (0 = reset)
	writeCommand(SSD1306_WIDTH - 1); // Column end address (127 = reset)
	// set line
	writeCommand(SSD1306_CMD_PAGEADDR);
	writeCommand(y); // Page start address (0 = reset)
	writeCommand(SSD1306_HEIGHT / 8 - 1); // Page end address	
}


void ssd1306_128x64_i2c::clearDisplay() {
	setPos(0,0);

    for (int i=0; i<SSD1306_WIDTH * SSD1306_HEIGHT / 8; i++) {  
    	Wire.beginTransmission(m_i2caddr);
        Wire.write(0x40);
    	Wire.write (0x0); 
    	Wire.endTransmission();	
    }
}

void ssd1306_128x64_i2c::writeCommand(uint8_t c) {
    Wire.beginTransmission(m_i2caddr);
    Wire.write(0x00); // control
    Wire.write(c);
    Wire.endTransmission();
}

void ssd1306_128x64_i2c::invertDisplay(uint8_t i) {
	if (i) {
		writeCommand(SSD1306_CMD_INVERTDISPLAY);
	} else {
		writeCommand(SSD1306_CMD_NORMALDISPLAY);
	}
}

void ssd1306_128x64_i2c::contrast(uint8_t contrast) {
  writeCommand(SSD1306_CMD_SETCONTRAST);
  writeCommand(contrast);
}

void ssd1306_128x64_i2c::drawPixel(int8_t x, int8_t y, uint8_t color) {
	setPos(x, y / 8);

    Wire.beginTransmission(m_i2caddr);
    Wire.write(0x40); // control
    if (color) 
    	Wire.write(1<<y%8);
    else
    	Wire.write(~(1<<y%8));
    Wire.endTransmission();
}

void ssd1306_128x64_i2c::drawCircle(int8_t x, int8_t y, int8_t radius, int8_t color) {
    int16_t xp = 0, yp = radius;
    int16_t d = 3 - (2 * radius);
    while(xp <= yp) {
        drawPixel(x + xp, y + yp, color);
        drawPixel(x + yp, y + xp, color);
        drawPixel(x - xp, y + yp, color);
        drawPixel(x + yp, y - xp, color);
        drawPixel(x - xp, y - yp, color);
        drawPixel(x - yp, y - xp, color);
        drawPixel(x + xp, y - yp, color);
        drawPixel(x - yp, y + xp, color);
        if (d < 0)
        	d += (4 * xp) + 6;
        else {
            d += (4 * (xp - yp)) + 10;
            y -= 1;
        }
        xp++;
    }
}

void ssd1306_128x64_i2c::drawChar(int8_t x, int8_t row, char c) {
	if (row > SSD1306_HEIGHT / FONT_HEIGHT) return;

	setPos(x, row);
	// draw. optimisation:6th font line is set as 0, to lower font array size
	for (int8_t i=0; i < FONT_WIDTH; i++ ) {
		uint8_t line = (i == FONT_WIDTH-1)? 0 : pgm_read_byte(font5x8 + (c * (FONT_WIDTH - 1)) + i);
		

	    Wire.beginTransmission(m_i2caddr);
	    Wire.write(0x40); // control
	    Wire.write(line);
	    Wire.endTransmission();

	}
}

void ssd1306_128x64_i2c::drawString(uint8_t x, uint8_t row, const char *string) {
    unsigned char c;
    while ( (c = *string++) ) {
		drawChar(x, row, c);
        if(x <= SSD1306_WIDTH - 2 * FONT_WIDTH)
        	x +=  FONT_WIDTH;
        else {
        	x = 0;
        	row +=  FONT_HEIGHT;
        }
    }
}


