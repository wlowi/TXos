
#include "LcdWidget.h"

#define XY_TO_I( x, y) (x + y * width)

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

/* standard ascii 5x7 font */
static const unsigned char  font5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,   
    0x3E, 0x5B, 0x4F, 0x5B, 0x3E, 	
    0x3E, 0x6B, 0x4F, 0x6B, 0x3E, 	
    0x1C, 0x3E, 0x7C, 0x3E, 0x1C, 
    0x18, 0x3C, 0x7E, 0x3C, 0x18, 
    0x1C, 0x57, 0x7D, 0x57, 0x1C, 
    0x1C, 0x5E, 0x7F, 0x5E, 0x1C, 
    0x00, 0x18, 0x3C, 0x18, 0x00, 
    0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 
    0x00, 0x18, 0x24, 0x18, 0x00, 
    0xFF, 0xE7, 0xDB, 0xE7, 0xFF, 
    0x30, 0x48, 0x3A, 0x06, 0x0E, 
    0x26, 0x29, 0x79, 0x29, 0x26, 
    0x40, 0x7F, 0x05, 0x05, 0x07, 
    0x40, 0x7F, 0x05, 0x25, 0x3F, 
    0x5A, 0x3C, 0xE7, 0x3C, 0x5A, 
    0x7F, 0x3E, 0x1C, 0x1C, 0x08, 
    0x08, 0x1C, 0x1C, 0x3E, 0x7F, 
    0x14, 0x22, 0x7F, 0x22, 0x14, 
    0x5F, 0x5F, 0x00, 0x5F, 0x5F, 
    0x06, 0x09, 0x7F, 0x01, 0x7F, 
    0x00, 0x66, 0x89, 0x95, 0x6A, 
    0x60, 0x60, 0x60, 0x60, 0x60, 
    0x94, 0xA2, 0xFF, 0xA2, 0x94, 
    0x08, 0x04, 0x7E, 0x04, 0x08, 
    0x10, 0x20, 0x7E, 0x20, 0x10, 
    0x08, 0x08, 0x2A, 0x1C, 0x08, 
    0x08, 0x1C, 0x2A, 0x08, 0x08, 
    0x1E, 0x10, 0x10, 0x10, 0x10, 
    0x0C, 0x1E, 0x0C, 0x1E, 0x0C, 
    0x30, 0x38, 0x3E, 0x38, 0x30, 
    0x06, 0x0E, 0x3E, 0x0E, 0x06, 
    0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x5F, 0x00, 0x00, 
    0x00, 0x07, 0x00, 0x07, 0x00, 
    0x14, 0x7F, 0x14, 0x7F, 0x14, 
    0x24, 0x2A, 0x7F, 0x2A, 0x12, 
    0x23, 0x13, 0x08, 0x64, 0x62, 
    0x36, 0x49, 0x56, 0x20, 0x50, 
    0x00, 0x08, 0x07, 0x03, 0x00, 
    0x00, 0x1C, 0x22, 0x41, 0x00, 
    0x00, 0x41, 0x22, 0x1C, 0x00, 
    0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 
    0x08, 0x08, 0x3E, 0x08, 0x08, 
    0x00, 0x80, 0x70, 0x30, 0x00, 
    0x08, 0x08, 0x08, 0x08, 0x08, 
    0x00, 0x00, 0x60, 0x60, 0x00, 
    0x20, 0x10, 0x08, 0x04, 0x02, 
    0x3E, 0x51, 0x49, 0x45, 0x3E, 
    0x00, 0x42, 0x7F, 0x40, 0x00, 
    0x72, 0x49, 0x49, 0x49, 0x46, 
    0x21, 0x41, 0x49, 0x4D, 0x33, 
    0x18, 0x14, 0x12, 0x7F, 0x10, 
    0x27, 0x45, 0x45, 0x45, 0x39, 
    0x3C, 0x4A, 0x49, 0x49, 0x31, 
    0x41, 0x21, 0x11, 0x09, 0x07, 
    0x36, 0x49, 0x49, 0x49, 0x36, 
    0x46, 0x49, 0x49, 0x29, 0x1E, 
    0x00, 0x00, 0x14, 0x00, 0x00, 
    0x00, 0x40, 0x34, 0x00, 0x00, 
    0x00, 0x08, 0x14, 0x22, 0x41, 
    0x14, 0x14, 0x14, 0x14, 0x14, 
    0x00, 0x41, 0x22, 0x14, 0x08, 
    0x02, 0x01, 0x59, 0x09, 0x06, 
    0x3E, 0x41, 0x5D, 0x59, 0x4E, 
    0x7C, 0x12, 0x11, 0x12, 0x7C, 
    0x7F, 0x49, 0x49, 0x49, 0x36, 
    0x3E, 0x41, 0x41, 0x41, 0x22, 
    0x7F, 0x41, 0x41, 0x41, 0x3E, 
    0x7F, 0x49, 0x49, 0x49, 0x41, 
    0x7F, 0x09, 0x09, 0x09, 0x01, 
    0x3E, 0x41, 0x41, 0x51, 0x73, 
    0x7F, 0x08, 0x08, 0x08, 0x7F, 
    0x00, 0x41, 0x7F, 0x41, 0x00, 
    0x20, 0x40, 0x41, 0x3F, 0x01, 
    0x7F, 0x08, 0x14, 0x22, 0x41, 
    0x7F, 0x40, 0x40, 0x40, 0x40, 
    0x7F, 0x02, 0x1C, 0x02, 0x7F, 
    0x7F, 0x04, 0x08, 0x10, 0x7F, 
    0x3E, 0x41, 0x41, 0x41, 0x3E, 
    0x7F, 0x09, 0x09, 0x09, 0x06, 
    0x3E, 0x41, 0x51, 0x21, 0x5E, 
    0x7F, 0x09, 0x19, 0x29, 0x46, 
    0x26, 0x49, 0x49, 0x49, 0x32, 
    0x03, 0x01, 0x7F, 0x01, 0x03, 
    0x3F, 0x40, 0x40, 0x40, 0x3F, 
    0x1F, 0x20, 0x40, 0x20, 0x1F, 
    0x3F, 0x40, 0x38, 0x40, 0x3F, 
    0x63, 0x14, 0x08, 0x14, 0x63, 
    0x03, 0x04, 0x78, 0x04, 0x03, 
    0x61, 0x59, 0x49, 0x4D, 0x43, 
    0x00, 0x7F, 0x41, 0x41, 0x41, 
    0x02, 0x04, 0x08, 0x10, 0x20, 
    0x00, 0x41, 0x41, 0x41, 0x7F, 
    0x04, 0x02, 0x01, 0x02, 0x04, 
    0x40, 0x40, 0x40, 0x40, 0x40, 
    0x00, 0x03, 0x07, 0x08, 0x00, 
    0x20, 0x54, 0x54, 0x78, 0x40, 
    0x7F, 0x28, 0x44, 0x44, 0x38, 
    0x38, 0x44, 0x44, 0x44, 0x28, 
    0x38, 0x44, 0x44, 0x28, 0x7F, 
    0x38, 0x54, 0x54, 0x54, 0x18, 
    0x00, 0x08, 0x7E, 0x09, 0x02, 
    0x18, 0xA4, 0xA4, 0x9C, 0x78, 
    0x7F, 0x08, 0x04, 0x04, 0x78, 
    0x00, 0x44, 0x7D, 0x40, 0x00, 
    0x20, 0x40, 0x40, 0x3D, 0x00, 
    0x7F, 0x10, 0x28, 0x44, 0x00, 
    0x00, 0x41, 0x7F, 0x40, 0x00, 
    0x7C, 0x04, 0x78, 0x04, 0x78, 
    0x7C, 0x08, 0x04, 0x04, 0x78, 
    0x38, 0x44, 0x44, 0x44, 0x38, 
    0xFC, 0x18, 0x24, 0x24, 0x18, 
    0x18, 0x24, 0x24, 0x18, 0xFC, 
    0x7C, 0x08, 0x04, 0x04, 0x08, 
    0x48, 0x54, 0x54, 0x54, 0x24, 
    0x04, 0x04, 0x3F, 0x44, 0x24, 
    0x3C, 0x40, 0x40, 0x20, 0x7C, 
    0x1C, 0x20, 0x40, 0x20, 0x1C, 
    0x3C, 0x40, 0x30, 0x40, 0x3C, 
    0x44, 0x28, 0x10, 0x28, 0x44, 
    0x4C, 0x90, 0x90, 0x90, 0x7C, 
    0x44, 0x64, 0x54, 0x4C, 0x44, 
    0x00, 0x08, 0x36, 0x41, 0x00, 
    0x00, 0x00, 0x77, 0x00, 0x00, 
    0x00, 0x41, 0x36, 0x08, 0x00, 
    0x02, 0x01, 0x02, 0x04, 0x02, 
    0x3C, 0x26, 0x23, 0x26, 0x3C, 
    0x1E, 0xA1, 0xA1, 0x61, 0x12, 
    0x3A, 0x40, 0x40, 0x20, 0x7A, 
    0x38, 0x54, 0x54, 0x55, 0x59, 
    0x21, 0x55, 0x55, 0x79, 0x41, 
    0x21, 0x54, 0x54, 0x78, 0x41, 
    0x21, 0x55, 0x54, 0x78, 0x40, 
    0x20, 0x54, 0x55, 0x79, 0x40, 
    0x0C, 0x1E, 0x52, 0x72, 0x12, 
    0x39, 0x55, 0x55, 0x55, 0x59, 
    0x39, 0x54, 0x54, 0x54, 0x59, 
    0x39, 0x55, 0x54, 0x54, 0x58, 
    0x00, 0x00, 0x45, 0x7C, 0x41, 
    0x00, 0x02, 0x45, 0x7D, 0x42, 
    0x00, 0x01, 0x45, 0x7C, 0x40, 
    0xF0, 0x29, 0x24, 0x29, 0xF0, 
    0xF0, 0x28, 0x25, 0x28, 0xF0, 
    0x7C, 0x54, 0x55, 0x45, 0x00, 
    0x20, 0x54, 0x54, 0x7C, 0x54, 
    0x7C, 0x0A, 0x09, 0x7F, 0x49, 
    0x32, 0x49, 0x49, 0x49, 0x32, 
    0x32, 0x48, 0x48, 0x48, 0x32, 
    0x32, 0x4A, 0x48, 0x48, 0x30, 
    0x3A, 0x41, 0x41, 0x21, 0x7A, 
    0x3A, 0x42, 0x40, 0x20, 0x78, 
    0x00, 0x9D, 0xA0, 0xA0, 0x7D, 
    0x39, 0x44, 0x44, 0x44, 0x39, 
    0x3D, 0x40, 0x40, 0x40, 0x3D, 
    0x3C, 0x24, 0xFF, 0x24, 0x24, 
    0x48, 0x7E, 0x49, 0x43, 0x66, 
    0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 
    0xFF, 0x09, 0x29, 0xF6, 0x20, 
    0xC0, 0x88, 0x7E, 0x09, 0x03, 
    0x20, 0x54, 0x54, 0x79, 0x41, 
    0x00, 0x00, 0x44, 0x7D, 0x41, 
    0x30, 0x48, 0x48, 0x4A, 0x32, 
    0x38, 0x40, 0x40, 0x22, 0x7A, 
    0x00, 0x7A, 0x0A, 0x0A, 0x72, 
    0x7D, 0x0D, 0x19, 0x31, 0x7D, 
    0x26, 0x29, 0x29, 0x2F, 0x28, 
    0x26, 0x29, 0x29, 0x29, 0x26, 
    0x30, 0x48, 0x4D, 0x40, 0x20, 
    0x38, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x38, 
    0x2F, 0x10, 0xC8, 0xAC, 0xBA, 
    0x2F, 0x10, 0x28, 0x34, 0xFA, 
    0x00, 0x00, 0x7B, 0x00, 0x00, 
    0x08, 0x14, 0x2A, 0x14, 0x22, 
    0x22, 0x14, 0x2A, 0x14, 0x08, 
    0xAA, 0x00, 0x55, 0x00, 0xAA, 
    0xAA, 0x55, 0xAA, 0x55, 0xAA, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 
    0x10, 0x10, 0x10, 0xFF, 0x00, 
    0x14, 0x14, 0x14, 0xFF, 0x00, 
    0x10, 0x10, 0xFF, 0x00, 0xFF, 
    0x10, 0x10, 0xF0, 0x10, 0xF0, 
    0x14, 0x14, 0x14, 0xFC, 0x00, 
    0x14, 0x14, 0xF7, 0x00, 0xFF, 
    0x00, 0x00, 0xFF, 0x00, 0xFF, 
    0x14, 0x14, 0xF4, 0x04, 0xFC, 
    0x14, 0x14, 0x17, 0x10, 0x1F, 
    0x10, 0x10, 0x1F, 0x10, 0x1F, 
    0x14, 0x14, 0x14, 0x1F, 0x00, 
    0x10, 0x10, 0x10, 0xF0, 0x00, 
    0x00, 0x00, 0x00, 0x1F, 0x10, 
    0x10, 0x10, 0x10, 0x1F, 0x10, 
    0x10, 0x10, 0x10, 0xF0, 0x10, 
    0x00, 0x00, 0x00, 0xFF, 0x10, 
    0x10, 0x10, 0x10, 0x10, 0x10, 
    0x10, 0x10, 0x10, 0xFF, 0x10, 
    0x00, 0x00, 0x00, 0xFF, 0x14, 
    0x00, 0x00, 0xFF, 0x00, 0xFF, 
    0x00, 0x00, 0x1F, 0x10, 0x17, 
    0x00, 0x00, 0xFC, 0x04, 0xF4, 
    0x14, 0x14, 0x17, 0x10, 0x17, 
    0x14, 0x14, 0xF4, 0x04, 0xF4, 
    0x00, 0x00, 0xFF, 0x00, 0xF7, 
    0x14, 0x14, 0x14, 0x14, 0x14, 
    0x14, 0x14, 0xF7, 0x00, 0xF7, 
    0x14, 0x14, 0x14, 0x17, 0x14, 
    0x10, 0x10, 0x1F, 0x10, 0x1F, 
    0x14, 0x14, 0x14, 0xF4, 0x14, 
    0x10, 0x10, 0xF0, 0x10, 0xF0, 
    0x00, 0x00, 0x1F, 0x10, 0x1F, 
    0x00, 0x00, 0x00, 0x1F, 0x14, 
    0x00, 0x00, 0x00, 0xFC, 0x14, 
    0x00, 0x00, 0xF0, 0x10, 0xF0, 
    0x10, 0x10, 0xFF, 0x10, 0xFF, 
    0x14, 0x14, 0x14, 0xFF, 0x14, 
    0x10, 0x10, 0x10, 0x1F, 0x00, 
    0x00, 0x00, 0x00, 0xF0, 0x10, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
    0x38, 0x44, 0x44, 0x38, 0x44, 
    0x7C, 0x2A, 0x2A, 0x3E, 0x14, 
    0x7E, 0x02, 0x02, 0x06, 0x06, 
    0x02, 0x7E, 0x02, 0x7E, 0x02, 
    0x63, 0x55, 0x49, 0x41, 0x63, 
    0x38, 0x44, 0x44, 0x3C, 0x04, 
    0x40, 0x7E, 0x20, 0x1E, 0x20, 
    0x06, 0x02, 0x7E, 0x02, 0x02, 
    0x99, 0xA5, 0xE7, 0xA5, 0x99, 
    0x1C, 0x2A, 0x49, 0x2A, 0x1C, 
    0x4C, 0x72, 0x01, 0x72, 0x4C, 
    0x30, 0x4A, 0x4D, 0x4D, 0x30, 
    0x30, 0x48, 0x78, 0x48, 0x30, 
    0xBC, 0x62, 0x5A, 0x46, 0x3D, 
    0x3E, 0x49, 0x49, 0x49, 0x00, 
    0x7E, 0x01, 0x01, 0x01, 0x7E, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x44, 0x44, 0x5F, 0x44, 0x44, 
    0x40, 0x51, 0x4A, 0x44, 0x40, 
    0x40, 0x44, 0x4A, 0x51, 0x40, 
    0x00, 0x00, 0xFF, 0x01, 0x03, 
    0xE0, 0x80, 0xFF, 0x00, 0x00, 
    0x08, 0x08, 0x6B, 0x6B, 0x08,
    0x36, 0x12, 0x36, 0x24, 0x36, 
    0x06, 0x0F, 0x09, 0x0F, 0x06, 
    0x00, 0x00, 0x18, 0x18, 0x00, 
    0x00, 0x00, 0x10, 0x10, 0x00, 
    0x30, 0x40, 0xFF, 0x01, 0x01, 
    0x00, 0x1F, 0x01, 0x01, 0x1E, 
    0x00, 0x19, 0x1D, 0x17, 0x12, 
    0x00, 0x3C, 0x3C, 0x3C, 0x3C, 
    0x00, 0x00, 0x00, 0x00, 0x00, 
};

LcdWidget::LcdWidget( wxWindow *parent, wxWindowID id, unsigned int width, unsigned int height, unsigned int pixSz)
    : wxPanel( parent, id, wxDefaultPosition, wxSize(width*pixSz,height*pixSz))
{
    this->width = width;
    this->height = height;
    this->pixSz = pixSz;
    textX = 0; // text cursor position
    textY = 0;
    buffer = new pixel[width*height];

    Connect(wxEVT_PAINT, wxPaintEventHandler(LcdWidget::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(LcdWidget::OnSize)); 

    setFg( 255, 255, 255);
    setBg( 0, 0, 0);
    setFontSize( 1);
    clear();
}

void LcdWidget::OnSize( wxSizeEvent& event)
{
    Refresh();
}

void LcdWidget::OnPaint( wxPaintEvent& event)
{
    unsigned int x;
    unsigned int y;
    pixel col565 = buffer[0];

    wxPaintDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    wxColor col = col565ToCol( col565);
    dc.SetPen(wxPen(col));
    dc.SetBrush(wxBrush(col));

    for( x=0; x<width; x++) {
        for( y=0; y<height; y++) {

            if( col565 != buffer[ XY_TO_I(x,y) ]) {
                col565 = buffer[ XY_TO_I(x,y) ];
                col = col565ToCol( col565);
                dc.SetPen(wxPen(col));
                dc.SetBrush(wxBrush(col));
            }

            setPixelOnDC( dc, x, y);
        }
    }
}

void LcdWidget::clear()
{
    unsigned int x;
    unsigned int y;

    for( x=0; x<width; x++) {
        for( y=0; y<height; y++) {
            buffer[ XY_TO_I(x,y) ] = bgCol565;
        }
    }

    textX = textY = 0;
    
    Refresh();
}

void LcdWidget::clearEOL()
{
    unsigned int x;
    unsigned int y;

    for( x=textX; x<width; x++) {
        for( y=textY; y<textY + (8*fontSz); y++) {
            buffer[ XY_TO_I(x,y) ] = bgCol565;
        }
    }
    
    Refresh();
}

void LcdWidget::setBg( unsigned char r, unsigned char g, unsigned char b)
{
    bgCol565 = rgbToCol565( r, g ,b);
    bgCol = wxColor( r, g, b);
}

void LcdWidget::setFg( unsigned char r, unsigned char g, unsigned char b)
{
    fgCol565 = rgbToCol565( r, g ,b);
    fgCol = wxColor( r, g, b);
}

void LcdWidget::setFontSize( unsigned int sz)
{
    if( sz < 1) { fontSz = 1; }
    else if( sz > 3) { fontSz = 3; }
    else { fontSz = sz; }
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
}

void LcdWidget::setRow( unsigned int r)
{
    textY = r * FONT_H;
    if( textY > height) { textY = height; }
}

void LcdWidget::setColumn( unsigned int c)
{
    textX = c * FONT_W;
    if( textX > width) { textX = width; }
}

void LcdWidget::setPixel( unsigned int x, unsigned int y)
{
    if( x >= width || y >= height) {
        return;
    }

    wxClientDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    dc.SetPen(wxPen(fgCol));
    dc.SetBrush(wxBrush(fgCol));

    buffer[ XY_TO_I(x,y) ] = fgCol565;
    setPixelOnDC( dc, x, y);    
}

void LcdWidget::setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
    if( x >= width || y >= height) {
        return;
    }

    wxClientDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    pixel col565 = rgbToCol565( r, g, b);
    wxColor col = wxColor( r, g, b);

    dc.SetPen(wxPen(col));
    dc.SetBrush(wxBrush(col));

    buffer[ XY_TO_I(x,y) ] = col565;
    setPixelOnDC( dc, x, y);
}

void LcdWidget::print( const char str[])
{
    if( str == NULL) {
        return;
    }

    wxClientDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    while( *str != '\0') {
        printChar( dc, *str);
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
  
  printIntGeneric( uval, neg, width, 0, filler);
}

void LcdWidget::printUInt( unsigned int val) {

  printIntGeneric( val, 0, 0, 0, ' ');
}

void LcdWidget::printUInt( unsigned int val, uint8_t width) {
  
  printIntGeneric( val, 0, width, 0, ' ');
}

void LcdWidget::printUInt( unsigned int val, uint8_t width, char filler) {
  
  printIntGeneric( val, 0, width, 0, filler);
}

void LcdWidget::printStr( const char str[], uint8_t width) {

    printStr( str, width, -1);
}

void LcdWidget::printStr( const char str[], uint8_t width, int8_t editIdx) {

  int8_t p = 0;

  if( !str) { return; }
  
  wxClientDC dc(this);
  dc.SetLogicalScale( pixSz, pixSz);

  while( str[p] && p < width) {
    if( p == editIdx) {
        saveColors();
        selectedColors();
    }
    printChar(dc,str[p]);
    if( p == editIdx) {
        restoreColors();
    }
    p++;
  }

  while( p < width) {
    printChar(dc,' ');
    p++;
  }
}

void LcdWidget::printFloat1( float1 val, uint8_t width) {

  printIntGeneric( val, 0, width, 1, ' ');
}

void LcdWidget::printFloat2( float2 val, uint8_t width) {

  printIntGeneric( val, 0, width, 2, ' ');
}

/* private */

void LcdWidget::setPixelOnDC( wxDC &dc, unsigned int x, unsigned int y)
{
    dc.DrawRectangle( x, y, 1, 1);
}

pixel LcdWidget::colToCol565( const wxColor &col)
{
    pixel col565 = ((col.Red() >> 3) << 11)
                 | ((col.Green() >> 2) << 5)
                 | (col.Blue() >> 3);

    return col565;
}

pixel LcdWidget::rgbToCol565( unsigned char r, unsigned char g, unsigned char b)
{
  pixel col565 = ((r >> 3) << 11)
               | ((g >> 2) << 5)
               | (b >> 3);

  return col565;  
}

wxColor LcdWidget::col565ToCol( pixel col565)
{
    unsigned char r = ((col565 >> 11) & 0x1f) << 3;
    unsigned char g = ((col565 >> 5) & 0x3f) << 2;
    unsigned char b = (col565 & 0x1f) << 3;

    return wxColor( r, g, b);
}

void LcdWidget::printChar( wxDC &dc, char ch)
{
    unsigned int x;
    unsigned int sz;

    for( x = 0; x < 5; x++) {
        for( sz = 0; sz < fontSz; sz++) {
            if( textX >= width) { break; }
            charLine( dc, font5x7[ 5 * ((unsigned char)ch) + x ]);
        }
    }

    for( sz = 0; sz < fontSz; sz++) {
        charLine( dc, 0); // Gap to next character
    }
}

void LcdWidget::charLine( wxDC &dc, char l)
 {
    unsigned int y;
    unsigned int sz;

    for( y = 0; y < 8; y++)	{
           if( l & 1) {
               dc.SetPen(wxPen(fgCol));
               dc.SetBrush(wxBrush(fgCol));
        } else {
               dc.SetPen(wxPen(bgCol));
               dc.SetBrush(wxBrush(bgCol));
        }

        for( sz = 0; sz < fontSz; sz++) {
            unsigned int ty = textY + sz + fontSz * y;

            if( ty >= height) { break; }

            if( l & 1) {
                buffer[ XY_TO_I(textX,ty) ] = fgCol565;
            } else {
                buffer[ XY_TO_I(textX,ty) ] = bgCol565;
            }

            setPixelOnDC( dc, textX, ty);
        }
        l = l >> 1;
    }

    textX++;
}

void LcdWidget::printIntGeneric( unsigned int val, int8_t neg, uint8_t width, uint8_t dot, char filler) {

    uint8_t bufflen = (width == 0) ? 6 : width;
    char buff[ 20 ];
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
    
      print( &buff[p] );
}

void LcdWidget::normalColors() {

    setBg(0,0,0);
    setFg(255,255,255);
}

void LcdWidget::selectedColors() {

    setBg(255,255,0);
    setFg(0,0,0);
}

void LcdWidget::headerColors() {

    setBg(0,0,0);
    setFg(0,255,0);
}

void LcdWidget::editColors() {

    setBg(255,255,255);
    setFg(0,0,0);
}

void LcdWidget::okColors() {

    setBg(0,0,0);
    setFg(0,255,0);
}

void LcdWidget::warnColors() {

    setBg(0,0,0);
    setFg(255,165,0);
}

void LcdWidget::alertColors() {

    setBg(0,0,0);
    setFg(255,0,0);
}

void LcdWidget::saveColors() {

    savedBgCol = bgCol565;
    savedFgCol = fgCol565;
}

void LcdWidget::restoreColors() {

    bgCol565 = savedBgCol;
    bgCol = col565ToCol(bgCol565);
    fgCol565 = savedFgCol;
    fgCol = col565ToCol(fgCol565);
}