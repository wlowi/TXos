
#include "SelectList.h"

void SelectList::clear() {

    table = nullptr;
    useBackItem = false;
    screenTableRows = 0;
    screenRow = 0;
    tableTopRow = 0;
    tableVisibleRows = 0;
    tableRow = 0;
    tableCol = 0;
    refresh = REFRESH_FULL;
    mode = MODE_RENDER;
}

void SelectList::set( TableEditable *tab, bool backItem) {

    clear();
    table = tab;
    useBackItem = backItem;
}

void SelectList::process( LcdWidget *lcd, Event *event) {

    uint8_t row; // real table row number, excluding back item

    if( refresh == REFRESH_FULL) {
        paint( lcd);
        refresh = REFRESH_OK;
    }

    switch( mode) {
    case MODE_RENDER:
        switch( event->key) {
        case KEY_ENTER:
            if( !(useBackItem && tableRow == 0)) {
                // table row always > 0
                row = useBackItem ? tableRow-1 : tableRow;
                if( table->isRowEditable( row)) {
                    LOGV("SelectList::process(): Is Row Editable (row=%d) => Y\n", row);
                    if( table->getColCount( row) > 0) {
                        mode = MODE_EDIT;
                        tableCol = 0;
                        table->getValue( row, tableCol, &cell);
                        refresh = REFRESH_UPDATE;
                    }
                }
                if( table->isRowExecutable( row)) {
                    LOGV("SelectList::process(): Is Row Executable (row=%d) => Y\n", row);
                    table->rowExecute( row);
                    refresh = REFRESH_UPDATE;
                }
            }
            break;

        case KEY_UP:
            next( event->count);
            refresh = REFRESH_UPDATE;
            break;

        case KEY_DOWN:
            prev( event->count);
            refresh = REFRESH_UPDATE;
            break;

        default:
            for( uint8_t tRow = tableTopRow; tRow < tableTopRow + tableVisibleRows; tRow++) {

                if( useBackItem && tRow == 0) {
                    continue;
                }

                row = useBackItem ? tRow-1 : tRow;

                for( uint8_t col = 0; col < table->getColCount(row); col++) {
                    if( table->hasChanged( row, col)) {
                        refreshLine( lcd, tRow);
                    }        
                }
            }
            break;
        }   
        break;

    case MODE_EDIT:

        row = useBackItem ? tableRow-1 : tableRow;

        cell.edit( event);
        table->setValue( row, tableCol, &cell);
        refresh = REFRESH_CELL;

        if( event->pending()) {
            switch( event->key) {

            case KEY_ENTER:
                tableCol++;
                if( tableCol >= table->getColCount( row)) {
                    mode = MODE_RENDER;
                    tableCol = 0;
                } else {
                    table->getValue( row, tableCol, &cell);
                }  
                refresh = REFRESH_UPDATE; 
                break;

            default:
                // ignore
                break;
            }
        }
        break;

    default:
        // ignore
        break;
    }

    if( refresh == REFRESH_UPDATE || refresh == REFRESH_CELL) {
        update( lcd);
        event->markProcessed();
        refresh = REFRESH_OK;
    }
}

void SelectList::prev( uint8_t count) {

    if( count >= tableRow) {
        tableRow = 0;
    } else {
        tableRow -= count;
    }
}

void SelectList::next( uint8_t count) {

    uint8_t rows = table->getRowCount() + (useBackItem ? 1 : 0);

    if( (tableRow + count) >= (rows - 1) ) {
        tableRow = rows-1;
    } else {
        tableRow += count;
    }
}

uint8_t SelectList::current() const {

    if( useBackItem) {
        return (tableRow == 0) ? GO_BACK : tableRow -1;
    }

    return tableRow;
}

/* private */

void SelectList::paint( LcdWidget *lcd) {

    const char *header = table->getName();

    lcd->setBg(0,0,0);
    lcd->clear();

    if( header) {
        headerColors( lcd);
        lcd->setCursor( 0, 0);
        lcd->print(header);
    }

    adjustTopRow( lcd);
    
    for( uint8_t row = tableTopRow; row < tableTopRow + tableVisibleRows; row++) {        
        refreshLine( lcd, row);
    }

    tableOldRow = tableRow;
}

void SelectList::update( LcdWidget *lcd) {

    if( adjustTopRow( lcd)) {
        paint( lcd);

    } else {
        refreshLine( lcd, tableOldRow);
        refreshLine( lcd, tableRow);
        tableOldRow = tableRow;
    }
}

/* Recomputes
 *  screenTableRows
 *  screenHeaderOffs
 *  tableTopRow
 *  tableVisibleRows
 */
bool SelectList::adjustTopRow( LcdWidget *lcd) {

    screenHeaderOffs = table->getName() ? 1 : 0;
    screenTableRows = lcd->getLines() - screenHeaderOffs;

    uint8_t rows = table->getRowCount() + (useBackItem ? 1 : 0);

    tableVisibleRows = (rows < screenTableRows) ? rows : screenTableRows;

    if( tableRow < tableTopRow) {
        tableTopRow = tableRow;
    } else if( tableRow >= (tableTopRow + screenTableRows)) {
        tableTopRow = tableRow - (screenTableRows -1);
    } else {
        return false;
    }

    return true;
}

/* Refresh a single line on the screen.
 * row is the table row number including back item.
 */
void SelectList::refreshLine( LcdWidget *lcd, uint8_t row) {

    (tableRow == row) ? selectedColors( lcd) : normalColors( lcd);
        
    lcd->setCursor( row - tableTopRow + screenHeaderOffs, 0);
    printLine( lcd, row);
}

/* Print a single line on screen.
 * row is the table row number including back item.
 */
void SelectList::printLine( LcdWidget *lcd, uint8_t row) {

    Cell renderCell;

    if( useBackItem) {
        if( row == 0) {
            lcd->print(TEXT_BACK);
            return;
        } else {
            row--;
        }
    }

    if( refresh != REFRESH_CELL) {
        lcd->print( table->getRowName(row));
    }

    for( uint8_t col = 0; col < table->getColCount(row); col++) {
        table->getValue( row, col, &renderCell);
        normalColors( lcd);

        if( mode == MODE_EDIT && col == tableCol) {
            editColors( lcd);
        } else {
            normalColors( lcd);
        }
        renderCell.render( lcd);
    }
}

void SelectList::normalColors( LcdWidget *lcd) const {

    lcd->setBg(0,0,0);
    lcd->setFg(255,255,255);
}

void SelectList::selectedColors( LcdWidget *lcd) const {

    lcd->setBg(255,255,0);
    lcd->setFg(0,0,0);
}

void SelectList::headerColors( LcdWidget *lcd) const {

    lcd->setBg(0,0,0);
    lcd->setFg(0,255,0);
}

void SelectList::editColors( LcdWidget *lcd) const {

    lcd->setBg(255,255,255);
    lcd->setFg(0,0,0);
}
