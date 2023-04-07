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
