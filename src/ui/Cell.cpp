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

#include "TableEditable.h"

extern Controls controls;

void Cell::render( LcdWidget *lcd, bool edit) {

    char switchName[TEXT_SW_NAME_STATE_length+1];

    lcd->setColumn( screenCol);

    if( edit) {
        lcd->editColors();
    } else {
        lcd->normalColors();
    }

    switch( type) {
        case BOOLEAN_T:
            if( value.boolV) {
                lcd->printStr( TEXT_ON, TEXT_ON_OFF_length);
            } else {
                lcd->printStr( TEXT_OFF, TEXT_ON_OFF_length);
            }
            break;

        case INT8_T:
            lcd->printInt( value.intV, (value.size == 0) ? 4 : value.size);
            break;

        case INT16_T:
            lcd->printInt( value.intV, (value.size == 0) ? 6 : value.size);
            break;

        case FLOAT1:
            lcd->printFloat1( value.intV, value.size);
            break;

        case FLOAT2:
            lcd->printFloat2( value.intV, value.size);
            break;

        case STRING_T:
            if( edit) {
                lcd->printStr( value.string, value.size, (int8_t)value.intV);
            } else {
                lcd->printStr( value.string, value.size);
            }
            break;

        case LABEL_T:
            lcd->printStr( value.label, value.size);
            break;

        case LIST_T:
            lcd->printStr( value.list[value.intV], value.size);
            break;

        case SWITCH_T:
            controls.copySwitchName( switchName, (switch_t)value.intV);
            lcd->printStr( switchName, TEXT_SW_NAME_length);
            break;

        case SWITCH_SET_STATE_T:
            controls.copySwitchNameAndState( switchName, (switch_t)value.intV);
            lcd->printStr( switchName, TEXT_SW_NAME_STATE_length);
            break;

        default:
            // ignore
            break;
    }
}

bool Cell::edit( Event *event) {

    switch_t sw;
    switchState_t swState;
    bool changed = false;

    if( type == SWITCH_SET_STATE_T && IS_SWITCH_USED( value.intV)) {
        swState = controls.switchGet(value.intV);

        if( swState != GET_SWITCH_STATE( value.intV)) {
            SET_SWITCH_STATE( value.intV, swState);
            changed = true;
        }
    }

    if( !event->pending()) {
        return changed;
    }

    switch( type) {
    case BOOLEAN_T:
        if( event->key == KEY_DOWN || event->key == KEY_UP) {
            value.boolV = ! value.boolV;
            event->markProcessed();
            changed = true;
        }
        break;

    case INT8_T:
    case INT16_T:
    case FLOAT1:
    case FLOAT2:
        if( event->key == KEY_DOWN) {
            value.intV -= event->count;
            event->markProcessed();
            changed = true;
        } else if( event->key == KEY_UP) {
            value.intV += event->count;
            event->markProcessed();
            changed = true;
        } else if( event->key == KEY_CLEAR) {
            value.intV = 0;
            event->markProcessed();
            changed = true;
        }

        if( value.intV > numericMax) {
            value.intV = numericMax;
        }
        if( value.intV < numericMin) {
            value.intV = numericMin;
        }

        break;

    case STRING_T:
        if( event->key == KEY_ENTER) {
            if( value.intV >= (value.size-1)) {
                /* intV = character index for editing.
                 * Done when we move beyond sting length.
                 */
                break;
            } else {
                /* Move to next character in string */
                value.intV++;
                event->markProcessed();
            }
            changed = true;
        } else if( event->key == KEY_DOWN) {
            /* Previous letter in alphabet. */
            value.string[value.intV] -= event->count;
            event->markProcessed();
            changed = true;
        } else if( event->key == KEY_UP) {
            /* Next letter in alphabet. */
            value.string[value.intV] += event->count;
            event->markProcessed();
            changed = true;
        } else if( event->key == KEY_CLEAR) {
            /* Space character. */
            value.string[value.intV] = ' ';
            event->markProcessed();
            changed = true;
        }

        if( value.string[value.intV] < ' ') {
            value.string[value.intV] = ' ';
        }
        if( value.string[value.intV] > (char)126) {
            value.string[value.intV] = (char)126;
        }

        break;

    case LIST_T:
        if( event->key == KEY_DOWN) {
            value.intV -= event->count;
            if( value.intV < 0) {
                value.intV = 0;
            }
            event->markProcessed();
            changed = true;
        } else if( event->key == KEY_UP) {
            value.intV += event->count;
            if( value.intV >= value.count) {
                value.intV = value.count-1;
            }
            event->markProcessed();
            changed = true;
        }
        break;

    case SWITCH_T:
    case SWITCH_SET_STATE_T:
        
        sw = GET_SWITCH(value.intV);

        if( event->key == KEY_DOWN) {
            
            if( IS_SWITCH_USED( value.intV)) {
                if( event->count > sw) {
                    sw = 0;
                    SET_SWITCH_UNUSED( value.intV);
                } else {
                    sw -= event->count;
                }
                SET_SWITCH( value.intV, sw);
            }
            event->markProcessed();
            changed = true;

        } else if( event->key == KEY_UP) {

            if( IS_SWITCH_UNUSED( value.intV)) {
                sw = event->count-1;
                SET_SWITCH_USED( value.intV);
            } else {
                sw += event->count;
            }
            if( sw >= SWITCHES) {
                sw = SWITCHES -1;
            }
            SET_SWITCH( value.intV, sw);            
            event->markProcessed();
            changed = true;

        } else if( event->key == KEY_CLEAR) {

            SET_SWITCH_UNUSED( value.intV);
            SET_SWITCH( value.intV, 0);
            event->markProcessed();
            changed = true;
        }
        break;
        
    default:
        // ignore
        break;
    }

    return changed;
}

bool Cell::isEditable() {

    return (type != LABEL_T);
}

void Cell::setBool( uint8_t screenX, bool v) {

    screenCol = screenX;
    type = BOOLEAN_T;
    value.boolV = v;
}

void Cell::setInt8( uint8_t screenX, int8_t v, uint8_t width, int16_t nmin, int16_t nmax) {

    screenCol = screenX;
    type = INT8_T;
    value.intV = v;
    value.size = width; /* display width */
    numericMin = nmin;
    numericMax = nmax;
}

void Cell::setInt16( uint8_t screenX, int16_t v, uint8_t width, int16_t nmin, int16_t nmax) {

    screenCol = screenX;
    type = INT16_T;
    value.intV = v;
    value.size = width; /* display width */
    numericMin = nmin;
    numericMax = nmax;
}

void Cell::setFloat1( uint8_t screenX, float1 v, uint8_t width, float1 nmin, float1 nmax) {

    screenCol = screenX;
    type = FLOAT1;
    value.intV = v;
    value.size = width; /* display width */
    numericMin = nmin;
    numericMax = nmax;
}

void Cell::setFloat2( uint8_t screenX, float2 v, uint8_t width, float2 nmin, float2 nmax) {

    screenCol = screenX;
    type = FLOAT2;
    value.intV = v;
    value.size = width; /* display width */
    numericMin = nmin;
    numericMax = nmax;
}

void Cell::setString( uint8_t screenX, char *v, uint8_t sz) {

    screenCol = screenX;
    type = STRING_T;
    value.string = v;
    value.size = sz;
    value.intV = 0; // character index for editing.
}

void Cell::setLabel( uint8_t screenX, const char *v, uint8_t sz) {

    screenCol = screenX;
    type = LABEL_T;
    value.label = v;
    value.size = sz;
}

void Cell::setList( uint8_t screenX, const char* const* v, uint8_t count, uint8_t curr) {

    uint8_t i;
    uint8_t sz;

    screenCol = screenX;
    type = LIST_T;
    value.list = v;
    value.count = count;
    value.intV = curr;

    /* determine max option string length */
    value.size = 0;
    for( i=0; i<count; i++) {
        sz = strlen( v[i]);
        if( sz > value.size) { value.size = sz; }
    }
}

void Cell::setSwitch( uint8_t screenX, switch_t v) {

    screenCol = screenX;
    type = SWITCH_T;
    value.intV = v;
}

void Cell::setSwitchState( uint8_t screenX, switch_t v) {

    screenCol = screenX;
    type = SWITCH_SET_STATE_T;
    value.intV = v;
}

bool Cell::getBool() const {

    return value.boolV;
}

int8_t Cell::getInt8() const {

    return (int8_t)value.intV;
}

int16_t Cell::getInt16() const {

    return value.intV;
}

float1 Cell::getFloat1() const {

    return value.intV;
}

float2 Cell::getFloat2() const {

    return value.intV;
}

char *Cell::getString() const {

    return value.string;
}

uint8_t Cell::getList() const {

    return (uint8_t)value.intV;
}

switch_t Cell::getSwitch() const {

    return (switch_t)value.intV;
}

switch_t Cell::getSwitchState() const {

    return (switch_t)value.intV;
}
