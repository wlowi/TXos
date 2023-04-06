/*
  TextUI. A simple text based UI.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef _TextUILcdSSD1306_h
#define _TextUILcdSSD1306_h

#include "TextUI.h"

#undef TEXTUI_LCD_USE_AVRI2C

#ifdef TEXTUI_LCD_USE_AVRI2C
  #include "SSD1306AsciiAvrI2c.h"
#else
  #include "SSD1306AsciiWire.h"
#endif

const uint8_t DISPLAY_I2C_ADDRESS = 0x3C;

class TextUILcdSSD1306 : public TextUILcd {

  private:
#ifdef TEXTUI_LCD_USE_AVRI2C
    SSD1306AsciiAvrI2c lcd;
#else
    SSD1306AsciiWire lcd;
#endif

  public:
    TextUILcdSSD1306();
    
    void clear();
    void clear( uint8_t x0, uint8_t r0, uint8_t x1, uint8_t r1);
    void clearEOL();

    bool colorSupport();
    
    void setBg( uint8_t r, uint8_t g, uint8_t b);
    void setFg( uint8_t r, uint8_t g, uint8_t b);

    void normalColors();
    void selectedColors();

    void setInvert( bool inv);

    /* FONT_SMALL .. FONT_LARGE */
    void setFontSize( uint8_t sz);
    
    uint16_t getRows();
    uint16_t getColumns();

    /* row and column in characters */
    void setCursor( uint8_t r, uint8_t c);
    void setRow( uint8_t r);
    void setColumn( uint8_t c);

    void printChar( char ch);

    void drawGrid( uint8_t dataArray[], uint8_t sz, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t grid, boolean marker);
};

#endif
