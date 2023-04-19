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

#include "TextUILcdST7735.h"

#include <SPI.h>

#define FONT_H (8 * fontSz)
#define FONT_W (6 * fontSz)

TextUILcdST7735::TextUILcdST7735( uint8_t tft_cs, uint8_t tft_dc, uint8_t tft_rst)
{
    tft = new Adafruit_ST7735(tft_cs, tft_dc, tft_rst);

    tft->initR(INITR_BLACKTAB);
    tft->setRotation( 3);
    
    width = tft->width();
    height = tft->height();

    textX = 0; // text cursor position
    textY = 0;

    setFg( 255, 255, 255);
    setBg( 0, 0, 0);
    setFontSize( TEXTUI_FONT_MEDIUM);
    invers = false;
    
    clear();
    setCursor(0, 0);
}

void TextUILcdST7735::clear()
{
    tft->fillScreen( invers ? fgCol565 : bgCol565);
    textX = textY = 0;
}

void TextUILcdST7735::clearEOL()
{
    tft->fillRect( textX, textY, width - textX, 8*fontSz, invers ? fgCol565 : bgCol565);
}

bool TextUILcdST7735::colorSupport() {

  return true;
}

void TextUILcdST7735::setBg( uint8_t r, uint8_t g, uint8_t b)
{
    bgCol565 = rgbToCol565( r, g ,b);
}

void TextUILcdST7735::setFg( uint8_t r, uint8_t g, uint8_t b)
{
    fgCol565 = rgbToCol565( r, g ,b);
}

void TextUILcdST7735::normalColors() {

    setBg(0,0,0);
    setFg(255,255,255);
}

void TextUILcdST7735::selectedColors() {

    setBg(255,255,0);
    setFg(0,0,0);
}

void TextUILcdST7735::editColors() {

    setBg(255,255,255);
    setFg(0,0,0);
}

void TextUILcdST7735::setInvert( bool inv) {

    invers = inv;
}

void TextUILcdST7735::setFontSize( FontSize_t sz)
{
    if( sz == TEXTUI_FONT_SMALL) {
        fontSz = 1;
    } else if( sz == TEXTUI_FONT_MEDIUM) {
        fontSz = 2;
    } else {
        fontSz = 3;
    }

    tft->setTextSize( fontSz);
}

uint8_t TextUILcdST7735::getRows() {

    return height / FONT_H;
}

uint8_t TextUILcdST7735::getColumns() {

    return width / FONT_W;
}

void TextUILcdST7735::setCursor(  uint8_t r, uint8_t c)
{
    textX = c * FONT_W;
    textY = r * FONT_H;

    if( textX > width) { textX = width; }
    if( textY > height) { textY = height; }

    tft->setCursor( textX, textY);
}

void TextUILcdST7735::setRow( uint8_t r)
{
    textY = r * FONT_H;
    if( textY > height) { textY = height; }
    tft->setCursor( textX, textY);
}

void TextUILcdST7735::setColumn( uint8_t c)
{
    textX = c * FONT_W;
    if( textX > width) { textX = width; }
    tft->setCursor( textX, textY);
}

void TextUILcdST7735::printChar( char ch)
{
    tft->drawChar( textX, textY, ch, invers ? bgCol565 : fgCol565, invers ? fgCol565 : bgCol565, fontSz);
    textX += FONT_W;
}

/* private */

pixel TextUILcdST7735::rgbToCol565( uint8_t r, uint8_t g, uint8_t b)
{
    pixel col565 = ((r >> 3) << 11)
                | ((g >> 2) << 5)
                | (b >> 3);

    return col565;  
}
