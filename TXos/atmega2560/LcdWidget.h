
#ifndef _LcdWidget_h_
#define _LcdWidget_h_

#include "Arduino.h"

typedef uint16_t pixel;

class LcdWidget
{
    private:
        unsigned int width;
        unsigned int height;
        pixel fgCol565;
        pixel bgCol565;
        unsigned int fontSz; // 1 - 3
        unsigned int textX;
        unsigned int textY;

        pixel rgbToCol565( unsigned char r, unsigned char g, unsigned char b);
        void printIntGeneric( unsigned int val, int8_t neg, uint8_t width, char filler);
        void printChar( char ch);

    public:
        LcdWidget();

        void clear();
        void setBg( unsigned char r, unsigned char g, unsigned char b);
        void setFg( unsigned char r, unsigned char g, unsigned char b);
        void setFontSize( unsigned int sz);
        unsigned int getLines();
        unsigned int getColumns();
        void setCursor( unsigned int r, unsigned int c);
        
        void print( const char str[]);
        void println( const char str[]);
        void println();

        void printInt( int val);
        void printInt( int val, uint8_t width);
        void printInt( int val, uint8_t width, char filler);

        void printUInt( unsigned int val);
        void printUInt( unsigned int val, uint8_t width);
        void printUInt( unsigned int val, uint8_t width, char filler);
};

#endif
