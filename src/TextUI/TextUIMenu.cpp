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

TextUIMenu::TextUIMenu( const char *hdr) : header( hdr)
{

}

TextUIMenu::TextUIMenu( const char *hdr, bool goBackItem) : header( hdr), useGoBackItem( goBackItem)
{

}

void TextUIMenu::addScreen( TextUIScreen *screenPtr) {

    screenPtr->menuNext = nullptr;

    if( first == nullptr) {
        first = last = screenPtr;
    } else {
        last->menuNext = screenPtr;
        last = screenPtr;
    }
}

TextUIScreen *TextUIMenu::getFirstScreen() {

    return first;
}

TextUIScreen *TextUIMenu::getNextScreen( TextUIScreen *scr) {

    return (scr == nullptr) ? nullptr : scr->menuNext;
}

/*
 * Return number of modules.
 * NOTE: This is stored nowhere. The method walks through the list of modules.
 *       This could be changed for efficiency.
 */
uint8_t TextUIMenu::getScreenCount() {

    TextUIScreen *current = first;
    uint8_t cnt = 0;

    while( current != nullptr) {
        cnt++;
        current = current->menuNext;
    }

    return cnt;
}

/*
 * Get a module by its index in the list.
 * NOTE: This funktion returns a nullptr if idx > number of modules in the list.
 */
TextUIScreen *TextUIMenu::getScreen( uint8_t idx) {

    TextUIScreen *current = first;
    uint8_t cnt = 0;

    while( current != nullptr) {
        if( cnt == idx) {
            break;
        }
        cnt++;
        current = current->menuNext;
    }

    return current;
}

/* Interface TableEditable */

const char *TextUIMenu::getHeader() {

    return header;
}

const char *TextUIMenu::getMenuName() {

    return getHeader();
}

bool TextUIMenu::isRowExecutable( uint8_t row) {

    return true; 
}

void TextUIMenu::rowExecute( TextUI *ui, uint8_t row ) {

    TextUIScreen *mod = getScreen( row);
    if( mod != nullptr) {
        setSelection( row);
        ui->pushScreen( mod);
    } 
}

uint8_t TextUIMenu::getRowCount() {

    return getScreenCount();
}

const char *TextUIMenu::getRowName( uint8_t row) {

    TextUIScreen *mod = getScreen( row);
    return (mod == nullptr) ? nullptr : mod->getMenuName();
}

uint8_t TextUIMenu::getColCount( uint8_t row) {

    return 0;
}

void TextUIMenu::getValue( uint8_t row, uint8_t col, Cell *cell) {
    // Nothing
}