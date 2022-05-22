
#include "TableEditable.h"

void Cell::render( LcdWidget *lcd) {

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
    }
}

void Cell::edit( Event *event) {

    if( event->pending()) {
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
        }
    }
}