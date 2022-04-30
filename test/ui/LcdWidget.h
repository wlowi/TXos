
#ifndef _LcdWidget_h_
#define _LcdWidget_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "Arduino.h"

typedef uint16_t pixel;

class LcdWidget : public wxPanel
{
    private:
        wxWindow *parent;
        unsigned int width;
        unsigned int height;
        unsigned int pixSz;
        pixel *buffer;
        pixel fgCol565;
        pixel bgCol565;
		wxColor fgCol;
		wxColor bgCol;
        unsigned int fontSz; // 1 - 3
        unsigned int textX;
        unsigned int textY;

        void setPixelOnDC( wxDC &dc, unsigned int x, unsigned int y);
        pixel colToCol565( const wxColor &col);
        pixel rgbToCol565( unsigned char r, unsigned char g, unsigned char b);
        wxColor col565ToCol( pixel col565);
        void printChar( wxDC &dc, char ch);
		void charLine( wxDC &dc, char l);

    public:
        LcdWidget( wxWindow *parent, wxWindowID id, unsigned int width, unsigned int height, unsigned int pixSz = 1);

        void OnSize( wxSizeEvent& event);
        void OnPaint( wxPaintEvent& event);

        void clear();
        void setBg( unsigned char r, unsigned char g, unsigned char b);
        void setFg( unsigned char r, unsigned char g, unsigned char b);
        void setFontSize( unsigned int sz);
        void setCursor( unsigned int r, unsigned int c);
        
        void setPixel( unsigned int x, unsigned int y);
        void setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
        void print( const char str[]);
        void println( const char str[]);        
};

#endif
