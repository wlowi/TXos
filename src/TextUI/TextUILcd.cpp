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

#include "TextUI.h"
#include "TXos.h"

void TextUILcd::printInt( int val) {
  
  printInt( val, 0, ' ');
}

void TextUILcd::printInt( int val, uint8_t width) {

  printInt( val, width, ' ');
}

void TextUILcd::printInt( int val, uint8_t width, char filler) {

  int8_t neg = 0;
  unsigned long uval;

  if( val < 0) {
    uval = -val;
    neg = 1;
  } else {
    uval = val;
  }

  printLongGeneric( uval, neg, width, 0, filler);
}

void TextUILcd::printUInt( unsigned int val)  {

  printLongGeneric( val, 0, 0, 0, ' ');
}

void TextUILcd::printUInt( unsigned int val, uint8_t width) {
  
  printLongGeneric( val, 0, width, 0, ' ');
}

void TextUILcd::printUInt( unsigned int val, uint8_t width, char filler) {
  
  printLongGeneric( val, 0, width, 0, filler);
}

void TextUILcd::printLong( long val) {

  printLong( val, 0, ' ');
}

void TextUILcd::printLong( long val, uint8_t width) {

  printLong( val, width, ' ');
}

void TextUILcd::printLong( long val, uint8_t width, char filler) {

  int8_t neg = 0;
  unsigned long uval;

  if( val < 0) {
    uval = -val;
    neg = 1;
  } else {
    uval = val;
  }

  printLongGeneric( uval, neg, width, 0, filler);
}

void TextUILcd::printULong( unsigned long val) {

  printLongGeneric( val, 0, 0, 0, ' ');
}

void TextUILcd::printULong( unsigned long val, uint8_t width) {

  printLongGeneric( val, 0, width, 0, ' ');
}

void TextUILcd::printULong( unsigned long val, uint8_t width, char filler) {
  
  printLongGeneric( val, 0, width, 0, filler);
}

void TextUILcd::printStr( const char str[]) {
  
  printStr( str, strlen( str), -1);
}

void TextUILcd::printStr( const char str[], uint8_t width) {
  
  printStr( str, width, -1);
}

void TextUILcd::printStr( const char str[], uint8_t width, int8_t editIdx) {
     
  uint8_t p = 0;

  if( !str) { return; }

  while( str[p] && p < width) {
    if( editIdx >= 0 && p == editIdx) {
      setInvert( true);
    }
    printChar( str[p]);
    if( editIdx >= 0 && p == editIdx) {
      setInvert( false);
    }
    p++;
  }

  while( p < width) {
    printChar( ' ');
    p++;
  } 

  yieldLoop();
}

#ifdef ARDUINO

void TextUILcd::printStr( const __FlashStringHelper *str) {
  
  printStr( str, 0);
}

void TextUILcd::printStr( const __FlashStringHelper *str, uint8_t width) {

  PGM_P p;
  unsigned char c;
  uint8_t n = 0;

  if( !str) { return; }

  p = reinterpret_cast<PGM_P>(str);

  while( (c = pgm_read_byte(p)) && (width == 0 || n < width)) {
    printChar( c);
    p++;
    n++;
  }

  while( width > 0 && n < width) {
    printChar( ' ');
    n++;
  }

  yieldLoop();
}

#endif

void TextUILcd::printFixFloat1( fixfloat1_t val, uint8_t width) {

  int8_t neg = 0;
  unsigned long uval;

  if( val < 0) {
    uval = -val;
    neg = 1;
  } else {
    uval = val;
  }
  
  printLongGeneric( uval, neg, width, 1, ' ');
}

void TextUILcd::printFixFloat2( fixfloat2_t val, uint8_t width) {

  int8_t neg = 0;
  unsigned long uval;

  if( val < 0) {
    uval = -val;
    neg = 1;
  } else {
    uval = val;
  }
  
  printLongGeneric( uval, neg, width, 2, ' ');
}

void TextUILcd::printLongGeneric( unsigned long val, int8_t neg, uint8_t width, uint8_t dot, char filler) {

  uint8_t bufflen = (width == 0) ? 12 : width;
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
      if( val == 0 && dot == 0) { // done
        break;
      } else if( p == 0) {
        neg = -1; // indicates overflow
        break;
      }

      p--;

      if( dot > 0) {
        dot--;
        if( dot == 0) {
          buff[p] = '.';
          if( p == 0) {
            break;
          }
          p--;
        }
      }
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

  printStr( &buff[p] );
}    
