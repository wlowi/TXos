/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef _SelectList_h_
#define _SelectList_h_

#include "TXos.h"
#include "TableEditable.h"
#include "DisplayImpl.h"

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_ROW     2
#define REFRESH_CELL    3

#define MODE_RENDER     0
#define MODE_EDIT       1

#define GO_BACK         255

class SelectList {

    private:
        TableEditable *table;
        bool useBackItem = false;
        Cell editCell;
        
        uint8_t screenTableRows;  /* Screen height available for table data
                                   * including back item, excluding header.
                                   */
        uint8_t screenHeaderOffs; /* Screen lines taken by row header.
                                   * Usually 0 or 1.
                                   */
        uint8_t tableRows;        /* Total number of table rows 
                                   * including back item.
                                   */
        uint8_t tableTopRow;      /* Table index of first displayed row 
                                   * including back item.
                                   */
        uint8_t tableVisibleRows; /* Number of visible table rows 
                                   * including back item.
                                   */

        uint8_t tableRow;         /* current table row, including back item*/
        uint8_t tableOldRow;
        uint8_t tableCol;         /* current table col */

        uint8_t refresh;
        uint8_t mode;

        bool adjustTopRow( LcdWidget *lcd);
        void refreshLine( LcdWidget *lcd, uint8_t i);
        void printLine( LcdWidget *lcd, uint8_t i);
        void paint( LcdWidget *lcd);
        void updateTable( LcdWidget *lcd);
        void updateRow( LcdWidget *lcd);
        void firstEditableCol( uint8_t row);
        void skipNonEditableCol( uint8_t row);
        void prev( uint8_t count);
        void next( uint8_t count);
        void clear();

    public:
        void set( TableEditable *tab, bool backItem);
        void set( TableEditable *tab, uint8_t currentSelected, bool backItem);

        void process( LcdWidget *lcd, Event *event);
        /* Cancel edit for this table */
        void cancelEdit( TableEditable *toCancel);
        
        uint8_t current() const;
};

#endif
