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

#if defined( ARDUINO_ARCH_AVR )
# include <Adafruit_GFX.h>    // Core graphics library
# include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#elif defined( ARDUINO_ARCH_ESP32 )
# include <TFT_eSPI.h>
#else
# error unknown architecture
#endif

typedef uint16_t pixel;

/**
 * @brief A driver for TFT displays with ST7735 controller.
 * 
 * Example:
 * 
 *      #include "TextUI.h"
 *      #include "TextUILcdST7735.h"
 * 
 *      #define TFT_CS        10
 *      #define TFT_DC         9
 *      #define TFT_RST       -1 // Or set to -1 and connect to Arduino RESET pin
 *      // It also uses the board specific hardware SPI pins.
 * 
 *      TextUI textUi;
 *      textUi.setDisplay( new TextUILcdST7735( TFT_CS, TFT_DC, TFT_RST));
 */
class TextUILcdST7735 : public TextUILcd
{
    private:
#if defined( ARDUINO_ARCH_AVR )
        Adafruit_ST7735 *tft;
#elif defined( ARDUINO_ARCH_ESP32 )
        TFT_eSPI *tft;    
#endif

        unsigned int width;
        unsigned int height;
        uint16_t font_h;
        uint16_t font_w;
        pixel fgCol565;
        pixel bgCol565;
        bool invers;
        uint8_t fontSz; // 1 - 3
        unsigned int textX;
        unsigned int textY;

        pixel rgbToCol565( uint8_t r, uint8_t g, uint8_t b);
        void initTFT();

    public:
      /**
       * @brief Construct a new TextUILcdST7735 driver.
       * 
       * This driver used the board specific hardware SPI interface 
       * and its corresponding pins.
       * 
       * Default display type is INITR_BLACKTAB supporting 160x128 pixel
       * 
       * @param tft_cs uint8_t: Chip select
       * @param tft_dc uint8_t: Data/Command select
       * @param tft_rst uint8_t: Reset pin ( can be -1 )
       */
      TextUILcdST7735( uint8_t tft_cs, uint8_t tft_dc, uint8_t tft_rst);

      /**
       * @brief Construct a new TextUILcdST7735 driver.
       * 
       * This driver used the board specific hardware SPI interface 
       * and its corresponding pins.
       * 
       * Display type options:
       * ~~~~
       *   INITR_GREENTAB           160x128
       *   INITR_144GREENTAB        128x128
       *   INITR_18GREENTAB         160x128
       *   INITR_REDTAB             160x128
       *   INITR_18REDTAB           160x128
       *   INITR_BLACKTAB           160x128
       *   INITR_18BLACKTAB         160x128
       *   INITR_MINI160x80         160x80
       *   INTTR_HALLOWING          128x128
       *   INITR_MINI160x80_PLUGIN  160x80
       * ~~~~
       * 
       * @param tft_cs uint8_t: Chip select
       * @param tft_dc uint8_t: Data/Command select
       * @param tft_rst uint8_t: Reset pin ( can be -1 )
       * @param options uint8_t: Display type option
       */
      TextUILcdST7735( uint8_t tft_cs, uint8_t tft_dc, uint8_t tft_rst, uint8_t option);
    
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
      void setFontSize( FontSize_t sz);
    
      uint8_t getRows();
      uint8_t getColumns();

      /* row and column in characters */
      void setCursor( uint8_t r, uint8_t c);
      void setRow( uint8_t r);
      void setColumn( uint8_t c);

      void printChar( char ch);
};

#endif
