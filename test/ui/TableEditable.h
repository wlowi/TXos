/*
 * Interface TableEditable
 */

#ifndef _TableEditable_h_
#define _TableEditable_h_

#include "TXos.h"
#include "DisplayImpl.h"

enum TableEditType_t {
    BOOLEAN_T,
    INT8_T,
    INT16_T,
    STRING_T,
    LIST_T,
};

typedef struct CellType_t {    
    
    bool boolV;
    
    int16_t intV;
    
    uint8_t size;
    const char *string;
    const char **list;

} CellType_t;

class Cell {

    public:
        TableEditType_t type;
        CellType_t value;
    
        void render( LcdWidget *lcd);
        void edit( Event *event);
};

class TableEditable {

    public:
        virtual const char *getName() = 0;

        virtual uint8_t getItemCount() = 0;
        virtual const char *getItemName( uint8_t row) = 0;

        virtual uint8_t getValueCount() = 0;

        virtual void getValue( uint8_t row, uint8_t col, Cell *cell) = 0;
        virtual void setValue( uint8_t row, uint8_t col, Cell *cell) = 0;
};

#endif
