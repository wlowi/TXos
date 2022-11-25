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
    
        uint8_t screenCol;

    public:
        void setBool( uint8_t screenX, bool v);
        void setInt8( uint8_t screenX, int8_t v, int16_t nmin, int16_t nmax);
        void setInt16( uint8_t screenX, int16_t v, int16_t nmin, int16_t nmax);
        void setString( uint8_t screenX, char *v, uint8_t sz);
        void setList( uint8_t screenX, const char **v, uint8_t sz, uint8_t curr);

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

        /* Return true if values of the row are editable */
        virtual bool isRowEditable( uint8_t row) { return true; }

        /* Return true if rowExecute should be called 
         * when the row is activated. 
         */
        virtual bool isRowExecutable( uint8_t row) { return false; }
        virtual void rowExecute( uint8_t row ) { /* default implementation does nothing */ }
        
        /* Check whether the module itself is executable. 
         * This will call moduleExecute() of the module if it is 
         * activated within a menu.
         */
        virtual bool isModuleExecutable() { return false; }
        /* Called when isModuleExecute() returns true and the user presses the ENTER key */
        virtual void moduleExecute() { /* default implementation does nothing */ }


        virtual uint8_t getRowCount() = 0;
        virtual const char *getRowName( uint8_t row) = 0;

        virtual uint8_t getColCount( uint8_t row) = 0;

        /* Returns true if a value has changed without user interaction */
        virtual bool hasChanged( uint8_t row, uint8_t col) { return false; }
        virtual void getValue( uint8_t row, uint8_t col, Cell *cell) = 0;
        virtual void setValue( uint8_t row, uint8_t col, Cell *cell) = 0;
};

#endif
