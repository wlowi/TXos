
#include "TableEditable.h"

void Cell::render( LcdWidget *lcd) const {

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
            lcd->print( value.string);
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
        }
        break;

    case STRING_T:
        if( event->key == KEY_ENTER) {
            if( value.intV >= (value.size-1)) {
                break; // done
            } else {
                value.intV++;
                event->markProcessed();
            }
        } else if( event->key == KEY_DOWN) {
            if( value.string[value.intV] > ' ') {
                value.string[value.intV]--;
                event->markProcessed();
            }
        } else if( event->key == KEY_UP) {
            if( value.string[value.intV] < 126) {
                value.string[value.intV]++;
                event->markProcessed();
            }
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

void Cell::setBool( bool v) {

    type = BOOLEAN_T;
    value.boolV = v;
}

void Cell::setInt8( int8_t v) {

    type = INT8_T;
    value.intV = v;
}

void Cell::setInt16( int16_t v) {

    type = INT16_T;
    value.intV = v;
}

void Cell::setString( char *v, uint8_t sz) {

    type = STRING_T;
    value.string = v;
    value.size = sz;
    value.intV = 0; // character index for editing.
}

void Cell::setList( const char **v, uint8_t sz, uint8_t curr) {

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
