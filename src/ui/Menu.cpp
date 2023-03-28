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

#include "Menu.h"

Menu::Menu( const char *hdr) : header( hdr)
{

}

void Menu::setDefaults() {

    LOG("Menu::setDefaults()\n");

    Module *current = first;

    while( current != nullptr) {
        current->setDefaults();
        current = current->menuNext;
    }
}

void Menu::init() {

    LOG("Menu::init()\n");

    Module *current = first;

    while( current != nullptr) {
        current->init();
        current = current->menuNext;
    }
}

void Menu::switchPhase( phase_t phase) {

    LOGV("Menu::switchPhase(): new phase %d\n", phase);

    Module *current = first;

    while( current != nullptr) {
        current->switchPhase( phase);
        current = current->menuNext;
    }
}

void Menu::addModule( Module *modulePtr) {

    modulePtr->menuNext = nullptr;

    if( first == nullptr) {
        first = last = modulePtr;
    } else {
        last->menuNext = modulePtr;
        last = modulePtr;
    }
}

Module *Menu::getFirstModule() {

    return first;
}

/*
 * Return number of modules.
 * NOTE: This is stored nowhere. The method walks through the list of modules.
 *       This could be changed for efficiency.
 */
uint8_t Menu::getModuleCount() {

    Module *current = first;
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
Module *Menu::getModule( uint8_t idx) {

    Module *current = first;
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

/*
 * Return a module by type.
 * NOTE: This method returns a nullptr if there is no module of that type.
 */
Module *Menu::getModuleByType( moduleType_t type) {

    Module *current = first;

    while( current != nullptr) {
        if( current->getConfigType() == type) {
            break;
        }
        current = current->menuNext;
    }

    return current;
}

/* Interface TableEditable */

bool Menu::isRowExecutable( uint8_t row) {

    Module *mod = getModule( row);
    return mod->isModuleExecutable(); 
}

void Menu::rowExecute( uint8_t row ) {

    Module *mod = getModule( row);
    mod->moduleExecute(); 
}

const char *Menu::getName() {

    return header;
}

uint8_t Menu::getRowCount() {

    return getModuleCount();
}

const char *Menu::getRowName( uint8_t row) {

    Module *mod = getModule( row);
    return mod->getName();
}

uint8_t Menu::getColCount( uint8_t row) {

    return 0;
}

void Menu::getValue( uint8_t row, uint8_t col, Cell *cell) {
    // Nothing
}

void Menu::setValue( uint8_t row, uint8_t col, Cell *cell) {
    // Nothing
}
