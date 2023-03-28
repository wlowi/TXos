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

#include "LcdWidget.h"

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

LcdWidget::LcdWidget( unsigned int width, unsigned int height, unsigned int pixSz) {
    fontSz = 2;
}

void LcdWidget::clear() {
}

void LcdWidget::clearEOL() {
}

void LcdWidget::setBg( unsigned char r, unsigned char g, unsigned char b) {
}

void LcdWidget::setFg( unsigned char r, unsigned char g, unsigned char b) {
}

void LcdWidget::setFontSize( unsigned int sz) {
}

unsigned int LcdWidget::getLines() {

    return lines[fontSz];
}

unsigned int LcdWidget::getColumns() {

    return columns[fontSz];
}

void LcdWidget::setCursor( unsigned int r, unsigned int c) {
}

void LcdWidget::setRow( unsigned int r) {
}

void LcdWidget::setColumn( unsigned int c) {
}

void LcdWidget::setPixel( unsigned int x, unsigned int y) {
}

void LcdWidget::setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b) {
}

void LcdWidget::print( const char str[]) {
}

void LcdWidget::println( const char str[]) {
}

void LcdWidget::println() {
}

void LcdWidget::printInt( int val) {
}

void LcdWidget::printInt( int val, uint8_t width) {
}

void LcdWidget::printInt( int val, uint8_t width, char filler) {
}

void LcdWidget::printUInt( unsigned int val) {
}

void LcdWidget::printUInt( unsigned int val, uint8_t width) {
}

void LcdWidget::printUInt( unsigned int val, uint8_t width, char filler) {
}

void LcdWidget::printStr( const char str[], uint8_t width) {
}

void LcdWidget::printStr( const char str[], uint8_t width, int8_t editIdx) {
}

void LcdWidget::printFloat1( float1 val, uint8_t width) {
}

void LcdWidget::printFloat2( float2 val, uint8_t width) {
}

void LcdWidget::normalColors() {
}

void LcdWidget::selectedColors() {
}

void LcdWidget::headerColors() {
}

void LcdWidget::editColors() {
}

void LcdWidget::okColors() {
}

void LcdWidget::warnColors() {
}

void LcdWidget::alertColors() {
}

void LcdWidget::saveColors() {
}

void LcdWidget::restoreColors() {
}
