
#include "TableEditable.h"

void Cell::render( LcdWidget *lcd, bool edit) const {

    lcd->setColumn( screenCol);

    if( edit) {
        lcd->editColors();
    } else {
        lcd->normalColors();
    }

    switch( type) {
        case BOOLEAN_T:
            if( value.boolV) {
                lcd->print( "Y");
            } else {
                lcd->print( "N");
            }
            break;

        case INT8_T:
            lcd->printInt( value.intV, 4);
            break;

        case INT16_T:
            lcd->printInt( value.intV, 6);
            break;

        case STRING_T:
            if( edit) {
                lcd->printStr( value.string, value.size, (int8_t)value.intV);
            } else {
                lcd->printStr( value.string, value.size);
            }
            break;

        case LIST_T:
            lcd->print( value.list[value.intV]);
            break;

        default:
            // ignore
            break;
    }
}

void Cell::edit( Event *event) {

    if( !event->pending()) {
        return;
    }

    switch( type) {
    case BOOLEAN_T:
        if( event->key == KEY_DOWN || event->key == KEY_UP) {
            value.boolV = ! value.boolV;
            event->markProcessed();
        }
        break;

    case INT8_T:
    case INT16_T:
        if( event->key == KEY_DOWN) {
            value.intV -= event->count;
            event->markProcessed();
        } else if( event->key == KEY_UP) {
            value.intV += event->count;
            event->markProcessed();
        } else if( event->key == KEY_CLEAR) {
            value.intV = 0;
            event->markProcessed();
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
        } else if( event->key == KEY_DOWN) {
            /* Previous letter in alphabet. */
            if( value.string[value.intV] > ' ') {
                value.string[value.intV]--;
                event->markProcessed();
            }
        } else if( event->key == KEY_UP) {
            /* Next letter in alphabet. */
            if( value.string[value.intV] < 126) {
                value.string[value.intV]++;
                event->markProcessed();
            }
        } else if( event->key == KEY_CLEAR) {
            /* Space character. */
            value.string[value.intV] = ' ';
            event->markProcessed();
        }
        break;

    case LIST_T:
        if( event->key == KEY_DOWN) {
            value.intV -= event->count;
            if( value.intV < 0) {
                value.intV = 0;
            }
            event->markProcessed();
        } else if( event->key == KEY_UP) {
            value.intV += event->count;
            if( value.intV >= value.size) {
                value.intV = value.size-1;
            }
            event->markProcessed();
        }
        break;

    default:
        // ignore
        break;
    }
}

void Cell::setBool( uint8_t screenX, bool v) {

    screenCol = screenX;
    type = BOOLEAN_T;
    value.boolV = v;
}

void Cell::setInt8( uint8_t screenX, int8_t v, int16_t nmin, int16_t nmax) {

    screenCol = screenX;
    type = INT8_T;
    value.intV = v;
    numericMin = nmin;
    numericMax = nmax;
}

void Cell::setInt16( uint8_t screenX, int16_t v, int16_t nmin, int16_t nmax) {

    screenCol = screenX;
    type = INT16_T;
    value.intV = v;
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

void Cell::setList( uint8_t screenX, const char **v, uint8_t sz, uint8_t curr) {

    screenCol = screenX;
    type = LIST_T;
    value.list = v;
    value.size = sz;
    value.intV = curr;
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

char *Cell::getString() const {

    return value.string;
}

uint8_t Cell::getList() const {

    return (uint8_t)value.intV;
}
