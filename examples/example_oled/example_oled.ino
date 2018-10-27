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

#include <Wire.h>
#include <ssd1306_128x64_i2c.h>

ssd1306_128x64_i2c display;


void setup()   {
    display.init();
    
    for (int i=0;i<128;i++)
      display.drawPixel(i,i/2,1);
    
    display.drawString(0, 7, "pocketmagic.net");

    display.drawString(50, 1, "Hello World!");
}

bool invert = 1;

void loop() {
  delay(1000);
  display.invertDisplay(invert);
  invert = !invert;
}

