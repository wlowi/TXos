/*
 * Interface TableEditable
 */

#ifndef _TableEditable_h_
#define _TableEditable_h_

#include "TXos.h"
#include "DisplayBox.h"

enum TableEditType_t {
    BOOLEAN_T,
    INT8_T
};

typedef union CellType_t {    
    bool boolV;
    int8_t int8V;
} CellType;

class Cell {

    TableEditType_t type;
    CellType value;

    public:
        void render( LcdWidget *lcd);
        void edit( LcdWidget *lcd, uint8_t key);
};

class TableEditable {

    public:
        virtual const char *getName() = 0;

        virtual uint8_t getItemCount() = 0;
        virtual const char *getItemName( uint8_t row) = 0;

        virtual uint8_t getValueCount() = 0;
        virtual TableEditType_t getValueType( uint8_t col) = 0;

        virtual void getValue( uint8_t row, uint8_t col, CellType *val) = 0;
        virtual void setValue( uint8_t row, uint8_t col, CellType *val) = 0;
};

#endif
