
#ifndef _SelectList_h_
#define _SelectList_h_

#include "TXos.h"
#include "TableEditable.h"
#include "DisplayBox.h"

#define SCREEN_INIT     0
#define SCREEN_HOME     1
#define SCREEN_SELECT   2
#define SCREEN_CONFIG   3

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_UPDATE  2

class SelectList {

    private:
        TableEditable *table;
        uint8_t idx;
        uint8_t oldIdx;
        uint8_t topIdx;

        bool adjustTopIdx( LcdWidget *lcd);
        void refreshLine( LcdWidget *lcd, uint8_t i);
        void printItem( LcdWidget *lcd, uint8_t i);
        void normalColors( LcdWidget *lcd);
        void selectedColors( LcdWidget *lcd);
        void headerColors( LcdWidget *lcd);
        
    public:
        void clear();
        void set( TableEditable *tab);
        void process( LcdWidget *lcd, Event *event);
        void prev();
        void next();
        uint8_t current();
        void paint( LcdWidget *lcd);
        void update( LcdWidget *lcd);        
};

#endif
