/*
  TXos. A remote control transmitter OS.

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

#ifndef _LcdWidget_h_
#define _LcdWidget_h_

#include "TXos.h"

class LcdWidget
{
    private:
        unsigned int fontSz; // 1 - 3
        unsigned int width;
        unsigned int height;

    public:
        LcdWidget( unsigned int width, unsigned int height, unsigned int pixSz = 1);

        /* Clear screen and move cursor to top-left position */
        void clear();
        /* Clear to end of line but do not move the cursor */
        void clearEOL();

        void setBg( unsigned char r, unsigned char g, unsigned char b);
        void setFg( unsigned char r, unsigned char g, unsigned char b);
        void setFontSize( unsigned int sz);
        unsigned int getLines();
        unsigned int getColumns();
        void setCursor( unsigned int r, unsigned int c);
        void setRow( unsigned int r);
        void setColumn( unsigned int c);
        
        void setPixel( unsigned int x, unsigned int y);
        void setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
        void print( const char str[]);
        void println( const char str[]);
        void println();

        void printInt( int val);
        void printInt( int val, uint8_t width);
        void printInt( int val, uint8_t width, char filler);

        void printUInt( unsigned int val);
        void printUInt( unsigned int val, uint8_t width);
        void printUInt( unsigned int val, uint8_t width, char filler);

        void printStr( const char str[], uint8_t width);
        void printStr( const char str[], uint8_t width, int8_t editIdx);

        void printFloat1( float1 val, uint8_t width);
        void printFloat2( float2 val, uint8_t width);

        void normalColors();
        void selectedColors();
        void headerColors();
        void editColors();
        void okColors();
        void warnColors();
        void alertColors();

        void saveColors();
        void restoreColors();
};

#endif
