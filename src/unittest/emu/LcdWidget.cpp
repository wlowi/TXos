/*
    TXos. A remote control transmitter OS.

    Copyright (C) 2022 Wolfgang Lohwasser

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

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