
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
            lcd->printInt( value.int8V, 4);
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
            if( event->key == KEY_DOWN) {
                value.int8V -= event->count;
                event->markProcessed();
            } else if( event->key == KEY_UP) {
                value.int8V += event->count;
                event->markProcessed();
            }
            break;
        }
    }
}