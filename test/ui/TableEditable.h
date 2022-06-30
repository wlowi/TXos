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
    char *string;
    const char **list;

} CellType_t;

class Cell {

    private:
        TableEditType_t type;
        CellType_t value;
        int16_t numericMin;
        int16_t numericMax;
    
    public:
        void setBool( bool v);
        void setInt8( int8_t v, int16_t nmin, int16_t nmax);
        void setInt16( int16_t v, int16_t nmin, int16_t nmax);
        void setString( char *v, uint8_t sz);
        void setList( const char **v, uint8_t sz, uint8_t curr);

        bool getBool() const;
        int8_t getInt8() const;
        int16_t getInt16() const;
        char *getString() const;
        uint8_t getList() const;

        void render( LcdWidget *lcd) const;
        void edit( Event *event);
};

class TableEditable {

    public:
        virtual const char *getName() = 0;

        virtual bool isEditable() { return true; }
        virtual bool isExecutable() { return false; }

        virtual void execute( uint8_t row ) { /* default implementation does nothing */ }

        virtual uint8_t getItemCount() = 0;
        virtual const char *getItemName( uint8_t row) = 0;

        virtual uint8_t getValueCount() = 0;

        virtual void getValue( uint8_t row, uint8_t col, Cell *cell) = 0;
        virtual void setValue( uint8_t row, uint8_t col, Cell *cell) = 0;
};

#endif
