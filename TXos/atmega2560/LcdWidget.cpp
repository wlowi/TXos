
#include "LcdWidget.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define FONT_H (8 * fontSz)
#define FONT_W (6 * fontSz)

/* 128 pixel, Base font height 8
 *   8 * 16 = 128
 *  16 *  8 = 128
 *  32 *  4 = 128
 */
static const unsigned int lines[] =  { 0, 16, 8, 4 };

/* 160 pixel, Base font width 6
 *   6 * 26 = 156
 *  12 * 13 = 156
 *  24 *  6 = 144
 */
static const unsigned int columns[] = { 0, 26, 13, 6 };

#define TFT_CS        10
#define TFT_RST        -1 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

LcdWidget::LcdWidget()
{
    tft.initR(INITR_BLACKTAB);
    tft.setRotation( 3);
    
    width = tft.width();
    height = tft.height();

    textX = 0; // text cursor position
    textY = 0;

    setFg( 255, 255, 255);
    setBg( 0, 0, 0);
	  setFontSize( 1);
    clear();
}

void LcdWidget::clear()
{
    tft.fillScreen( bgCol565);
	  textX = textY = 0;
}

void LcdWidget::setBg( unsigned char r, unsigned char g, unsigned char b)
{
    bgCol565 = rgbToCol565( r, g ,b);
}

void LcdWidget::setFg( unsigned char r, unsigned char g, unsigned char b)
{
    fgCol565 = rgbToCol565( r, g ,b);
}

void LcdWidget::setFontSize( unsigned int sz)
{
	if( sz < 1) { fontSz = 1; }
	else if( sz > 3) { fontSz = 3; }
	else { fontSz = sz; }

  tft.setTextSize( fontSz);
}

unsigned int LcdWidget::getLines() {

	return lines[fontSz];
}

unsigned int LcdWidget::getColumns() {

	return columns[fontSz];
}

void LcdWidget::setCursor( unsigned int r, unsigned int c)
{
	textX = c * FONT_W;
	textY = r * FONT_H;

	if( textX > width) { textX = width; }
	if( textY > height) { textY = height; }

  tft.setCursor( textX, textY);
}

void LcdWidget::print( const char str[])
{
    if( str == NULL) {
        return;
    }

    while( *str != '\0') {
        printChar( *str);
        str++;
    }
}

void LcdWidget::println( const char str[])
{
    print( str);
	  println();
}

void LcdWidget::println() {

    textX = 0;
    textY += FONT_H;
    textY = (textY > height) ? height: textY;
}

void LcdWidget::printInt( int val) {

  printInt( val, 0, ' ');
}

void LcdWidget::printInt( int val, uint8_t width) {

  printInt( val, width, ' ');
}

void LcdWidget::printInt( int val, uint8_t width, char filler) {

  int8_t neg = 0;
  unsigned int uval;

  if( val < 0) {
    uval = -val;
    neg = 1;
  } else {
    uval = val;
  }
  
  printIntGeneric( uval, neg, width, filler);
}

void LcdWidget::printUInt( unsigned int val) {

  printIntGeneric( val, 0, 0, ' ');
}

void LcdWidget::printUInt( unsigned int val, uint8_t width) {
  
  printIntGeneric( val, 0, width, ' ');
}

void LcdWidget::printUInt( unsigned int val, uint8_t width, char filler) {
  
  printIntGeneric( val, 0, width, filler);
}

/* private */

void LcdWidget::printChar( char ch)
{
  tft.drawChar( textX, textY, ch, fgCol565, bgCol565, fontSz);
  textX += FONT_W;
}

pixel LcdWidget::rgbToCol565( unsigned char r, unsigned char g, unsigned char b)
{
  pixel col565 = ((r >> 3) << 11)
               | ((g >> 2) << 5)
               | (b >> 3);

  return col565;  
}

void LcdWidget::printIntGeneric( unsigned int val, int8_t neg, uint8_t width, char filler) {

	uint8_t bufflen = (width == 0) ? 6 : width;
	char buff[ bufflen+1 ];
	uint8_t p = bufflen;

	buff[p--] = '\0';

  	// p >= 0 here
  	if( val == 0) {
    	buff[p] = '0';
  	} else {
    	for(;;) {
      		buff[p] = (val % 10) + '0';
      		val /= 10;
      		if( val == 0) { // done
        		break;
      		} else if( p == 0) {
        		neg = -1; // indicates overflow
        		break;
      		}
        
      		p--;
    	}
  	}

  	if( neg == 1) { // indicates negative sign
    	if( p == 0) {
      		neg = -1; // indicates overflow
    	} else {
      		p--;
      	buff[p] = '-';
    	}
  	}
    
  	if( neg < 0) { // indicates overflow
    	for( p = 0; p < bufflen; p++) {
      		buff[p] = '*'; 
    	}
    	p = 0;
  	} else {
	    if( width > 0) {
    		while( p > 0) {
        		p--;
        		buff[p] = filler;
      		}
    	}
  	}
    
  	print( &buff[p] );
}
