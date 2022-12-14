
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

        case FLOAT16:
            lcd->printFloat16( value.intV, value.size);
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

void Cell::edit( Event *event) {

    if( type == SWITCH_SET_STATE_T) {
        SET_SWITCH_STATE( value.intV, controls.switchGet(value.intV));
    }

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
    case FLOAT16:
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
            value.string[value.intV] -= event->count;
            event->markProcessed();
        } else if( event->key == KEY_UP) {
            /* Next letter in alphabet. */
            value.string[value.intV] += event->count;
            event->markProcessed();
        } else if( event->key == KEY_CLEAR) {
            /* Space character. */
            value.string[value.intV] = ' ';
            event->markProcessed();
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
        } else if( event->key == KEY_UP) {
            value.intV += event->count;
            if( value.intV >= value.count) {
                value.intV = value.count-1;
            }
            event->markProcessed();
        }
        break;

    case SWITCH_T:
    case SWITCH_SET_STATE_T:
        if( event->key == KEY_DOWN) {
            if( IS_SWITCH_USED( value.intV)) {
                value.intV -= event->count;
                if( value.intV < 0) {
                    SET_SWITCH( value.intV, 0);
                    SET_SWITCH_UNUSED( value.intV);
                }
            }
            event->markProcessed();
        } else if( event->key == KEY_UP) {
            if( IS_SWITCH_UNUSED( value.intV)) {
                value.intV = event->count-1;
                if( GET_SWITCH( value.intV) >= SWITCHES) {
                    SET_SWITCH( value.intV, SWITCHES -1);
                }
            } else {
                value.intV += event->count;
                if( GET_SWITCH( value.intV) >= SWITCHES) {
                    SET_SWITCH( value.intV, SWITCHES -1);
                }
            }            
            event->markProcessed();
        } else if( event->key == KEY_CLEAR) {
            SET_SWITCH_UNUSED( value.intV);
            event->markProcessed();
        }
        break;
        
    default:
        // ignore
        break;
    }
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

void Cell::setFloat16( uint8_t screenX, float16 v, uint8_t width, float16 nmin, float16 nmax) {

    screenCol = screenX;
    type = FLOAT16;
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

void Cell::setList( uint8_t screenX, const char **v, uint8_t count, uint8_t curr) {

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

float16 Cell::getFloat16() const {

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
