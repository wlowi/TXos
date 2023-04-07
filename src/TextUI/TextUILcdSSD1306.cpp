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

#include "TextUILcdSSD1306.h"

TextUILcdSSD1306::TextUILcdSSD1306() {

#ifdef TEXTUI_LCD_USE_AVRI2C

  delay(100);
  lcd.begin(&SH1106_128x64, DISPLAY_I2C_ADDRESS);

#else

  Wire.begin();
  Wire.setClock(400000L);

  Wire.beginTransmission( DISPLAY_I2C_ADDRESS);
  
  if (Wire.endTransmission() == 0) {
    lcd.begin(&SH1106_128x64, DISPLAY_I2C_ADDRESS);
  }

#endif
  delay(100);
    
  lcd.setFont(Adafruit5x7);
  setFontSize( TEXTUI_FONT_SMALL);           // Normal 1:1 pixel scale

  clear();
  setCursor(0, 0);
}

void TextUILcdSSD1306::clear() {

  lcd.clear();
}

void TextUILcdSSD1306::clear( uint8_t x0, uint8_t r0, uint8_t x1, uint8_t r1) {

  lcd.clear( x0, x1, r0, r1);
}

void TextUILcdSSD1306::clearEOL() {

  lcd.clearToEOL();
}

bool TextUILcdSSD1306::colorSupport() {

  return false;
}
    
void TextUILcdSSD1306::setBg( uint8_t r, uint8_t g, uint8_t b) {

  /* noop */
}

void TextUILcdSSD1306::setFg( uint8_t r, uint8_t g, uint8_t b) {

  /* noop */
}

void TextUILcdSSD1306::normalColors() {

  setInvert( false);
}

void TextUILcdSSD1306::selectedColors() {

  setInvert( true);
}

void TextUILcdSSD1306::editColors() {

  setInvert( true);
}

void TextUILcdSSD1306::setInvert( bool inv) {

  lcd.setInvertMode( inv);
}

void TextUILcdSSD1306::setFontSize( uint8_t sz) {

  if( sz == TEXTUI_FONT_SMALL) {
    lcd.set1X();
  } else {
    lcd.set2X();
  }
}
    
uint16_t TextUILcdSSD1306::getRows() {

  return lcd.displayHeight() / (8 * lcd.fontRows());
}

uint16_t TextUILcdSSD1306::getColumns() {

  return lcd.displayWidth() / (lcd.fontWidth() + lcd.letterSpacing());
}
    
void TextUILcdSSD1306::setCursor( uint8_t r, uint8_t c) {

  setRow( r);
  setColumn( c);
}

void TextUILcdSSD1306::setRow( uint8_t r) {

  lcd.setRow( r * lcd.fontRows());
}

void TextUILcdSSD1306::setColumn( uint8_t c) {

  lcd.setCol( c * (lcd.fontWidth() + lcd.letterSpacing()));
}

void TextUILcdSSD1306::printChar( char ch) {

  lcd.write( ch);
}

void TextUILcdSSD1306::drawGrid( uint8_t dataArray[], uint8_t sz, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t grid, boolean marker)
{
  uint8_t m;
  uint8_t y;
  uint8_t p;
  uint8_t sRow;
  uint8_t eRow;
  uint8_t d0Row;
  uint8_t d1Row;
  uint8_t d0;
  uint8_t d1;
  uint8_t r;
  uint8_t x;
  uint8_t i;
  
  sRow = y0 / 8;
  eRow = y1 / 8;

  for( r = sRow; r <= eRow; r++) {
    lcd.setCol( x0);
    lcd.setRow( r);

    i = 0;
    p = y1 - dataArray[i++];
    for( x = x0; (x <= x1) && (i < sz); x++) {
      y = y1 - dataArray[i++];

      if( y < p) {
        d0 = y; d1 = p;
      } else {
        d0 = p; d1 = y;
      }

      d0Row = d0 / 8;
      d1Row = d1 / 8;
      
      if( r == d0Row && d0Row == d1Row) {
        m = 255 - ((1 << (d0 % 8)) -1);
        m &= (1 << ((d1 % 8) +1)) -1;
      } else if( r == d0Row) {
        m = 255 - ((1 << (d0 % 8)) -1);
      } else if( r == d1Row) {
        m = (1 << ((d1 % 8) +1)) -1;
      } else if (r < d0Row || r > d1Row) {
        if( grid && ((x % grid) == 0)) {
          m = 0b10000000; 
        } else {
          m = 0;
        }
      } else {
        m = 0xff;
      }
      
      lcd.ssd1306WriteRam( m);
      p = y;
    }

    for( ; x <= x1; x++) {
      lcd.ssd1306WriteRam( 0);
    }
  }

  if( marker) {
    lcd.setCol( x1);
    lcd.setRow( sRow);
    lcd.ssd1306WriteRam( 0x0f);
  }
}
