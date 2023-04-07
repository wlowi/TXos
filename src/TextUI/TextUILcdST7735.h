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

#ifndef _TextUILcdST7735_h_
#define _TextUILcdST7735_h_

#include "TextUI.h"

typedef uint16_t pixel;

class TextUILcdST7735 : public TextUILcd
{
    private:
        unsigned int width;
        unsigned int height;
        pixel fgCol565;
        pixel bgCol565;
        bool invers;
        unsigned int fontSz; // 1 - 3
        unsigned int textX;
        unsigned int textY;

        pixel rgbToCol565( uint8_t r, uint8_t g, uint8_t b);

    public:
      TextUILcdST7735();
    
      void clear();
      void clearEOL();

      bool colorSupport();
    
      void setBg( uint8_t r, uint8_t g, uint8_t b);
      void setFg( uint8_t r, uint8_t g, uint8_t b);

      void normalColors();
      void selectedColors();
      void editColors();

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
};

#endif
