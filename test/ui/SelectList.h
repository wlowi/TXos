
#ifndef _SelectList_h_
#define _SelectList_h_

#include "TXos.h"
#include "TableEditable.h"
#include "DisplayImpl.h"

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_UPDATE  2
#define REFRESH_CELL    3

#define MODE_RENDER     0
#define MODE_EDIT       1

#define GO_BACK         255

class SelectList {

    private:
        TableEditable *table;
        bool useBackItem = false;
        Cell editCell;
        
        uint8_t screenTableRows;  /* screen height available for table data
                                   * including back item, excluding header
                                   */
        uint8_t screenHeaderOffs; /* screen lines taken by row header
                                   */
        uint8_t screenRow;        /* current screen row 
                                   * including back item, excluding header
                                   */
        uint8_t tableTopRow;      /* table index of first displayed row 
                                   * including back item
                                   */
        uint8_t tableVisibleRows; /* number of displayed table rows 
                                   * including back item
                                   */

        uint8_t tableRow;         /* current table row */
        uint8_t tableOldRow;
        uint8_t tableCol;         /* current table col */

        uint8_t refresh;
        uint8_t mode;

        bool adjustTopRow( LcdWidget *lcd);
        void refreshLine( LcdWidget *lcd, uint8_t i);
        void printLine( LcdWidget *lcd, uint8_t i);
        void paint( LcdWidget *lcd);
        void update( LcdWidget *lcd);
        void prev( uint8_t count);
        void next( uint8_t count);
        void clear();

    public:
        void set( TableEditable *tab, bool backItem);
        void process( LcdWidget *lcd, Event *event);
        uint8_t current() const;
};

#endif
