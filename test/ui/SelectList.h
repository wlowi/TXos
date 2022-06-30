
#ifndef _SelectList_h_
#define _SelectList_h_

#include "TXos.h"
#include "TableEditable.h"
#include "DisplayImpl.h"

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_UPDATE  2

#define MODE_RENDER     0
#define MODE_EDIT       1

class SelectList {

    private:
        TableEditable *table;
        Cell cell;
        uint8_t idx;
        uint8_t oldIdx;
        uint8_t topIdx;
        uint8_t refresh;
        uint8_t mode;
        uint8_t editCol;

        bool adjustTopIdx( LcdWidget *lcd);
        void refreshLine( LcdWidget *lcd, uint8_t i);
        void printItem( LcdWidget *lcd, uint8_t i);
        void paint( LcdWidget *lcd);
        void update( LcdWidget *lcd);
        void normalColors( LcdWidget *lcd) const;
        void selectedColors( LcdWidget *lcd) const;
        void headerColors( LcdWidget *lcd) const;
        void editColors( LcdWidget *lcd) const;
        
    public:
        void clear();
        void set( TableEditable *tab);
        void process( LcdWidget *lcd, Event *event);
        void prev( uint8_t count);
        void next( uint8_t count);
        uint8_t current() const;
};

#endif