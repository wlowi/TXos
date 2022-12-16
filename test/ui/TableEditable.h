/*
 * Interface TableEditable
 */

#ifndef _TableEditable_h_
#define _TableEditable_h_

#include "TXos.h"
#include "Controls.h"
#include "DisplayImpl.h"

enum TableEditType_t {
    BOOLEAN_T,
    INT8_T,
    INT16_T,
    FLOAT1,
    FLOAT2,
    STRING_T,
    LABEL_T,
    LIST_T,
    SWITCH_T,
    SWITCH_SET_STATE_T
};

typedef struct CellType_t {    
    
    bool boolV;
    
    int16_t intV;
    
    uint8_t size;        /* String length */
    char *string;
    const char *label;

    uint8_t count;       /* List size */
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
        void setInt8( uint8_t screenX, int8_t v, uint8_t width, int16_t nmin, int16_t nmax);
        void setInt16( uint8_t screenX, int16_t v, uint8_t width, int16_t nmin, int16_t nmax);
        void setFloat1( uint8_t screenX, float1 v, uint8_t width, float1 nmin, float1 nmax);
        void setFloat2( uint8_t screenX, float2 v, uint8_t width, float2 nmin, float2 nmax);
        void setString( uint8_t screenX, char *v, uint8_t sz);
        void setLabel( uint8_t screenX, const char *v, uint8_t sz);
        void setList( uint8_t screenX, const char **v, uint8_t sz, uint8_t curr);
        void setSwitch( uint8_t screenX, switch_t v);
        void setSwitchState( uint8_t screenX, switch_t v);

        bool getBool() const;
        int8_t getInt8() const;
        int16_t getInt16() const;
        float1 getFloat1() const;
        float2 getFloat2() const;
        char *getString() const;
        uint8_t getList() const;
        switch_t getSwitch() const;
        switch_t getSwitchState() const;

        void render( LcdWidget *lcd, bool edit);
        void edit( Event *event);
        bool isEditable();
};

class TableEditable {

    public:
        virtual const char *getName() = 0;

        /* Return true if values of the row are editable */
        virtual bool isRowEditable( uint8_t row) { return true; }
        /* Return true if the value of this cell is editable */
        virtual bool isColEditable( uint8_t row, uint8_t col) { return true; }

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

        /* Table requests full redraw */
        virtual bool needsRefresh() { return false; };

        /* Returns true if a value has changed without user interaction */
        virtual bool hasChanged( uint8_t row, uint8_t col) { return false; }
        virtual void getValue( uint8_t row, uint8_t col, Cell *cell) = 0;
        virtual void setValue( uint8_t row, uint8_t col, Cell *cell) = 0;
};

#endif
