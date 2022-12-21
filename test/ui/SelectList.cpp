
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

void SelectList::set( TableEditable *tab, uint8_t currentSelected, bool backItem) {

    set( tab, backItem);

    if( currentSelected == GO_BACK) {
        tableRow = 0;

    } else {

        if( currentSelected >= table->getRowCount()) {
            currentSelected = table->getRowCount()-1;
        }

        tableRow = backItem ? currentSelected+1 : currentSelected;
    }
}

void SelectList::process( LcdWidget *lcd, Event *event) {

    uint8_t row; // real table row number, excluding back item

    if( refresh == REFRESH_FULL) {
        paint( lcd);
        refresh = REFRESH_OK;
    } else if ( table->needsRefresh()) {
        updateTable( lcd);
    }

    switch( mode) {
    case MODE_RENDER:
        switch( event->key) {
        case KEY_ENTER:
            if( !(useBackItem && tableRow == 0)) {
                // tableRow is always > 0
                row = useBackItem ? tableRow-1 : tableRow;
                firstEditableCol( row);

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
                        refresh = REFRESH_CELL;
                        refreshLine( lcd, tRow);
                        refresh = REFRESH_OK;
                        break;
                    }        
                }
            }
            break;
        }   
        break;

    case MODE_EDIT:

        row = useBackItem ? tableRow-1 : tableRow;

        if( editCell.edit( event)) {
            table->setValue( row, tableCol, &editCell);
            refresh = REFRESH_CELL;
        } else {
            refresh = REFRESH_OK;
        }

        if( event->pending()) {
            switch( event->key) {

            case KEY_ENTER:
                tableCol++;
                skipNonEditableCol( row);
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
        updateRow( lcd);
        event->markProcessed();
        refresh = REFRESH_OK;
    }
}

void SelectList::cancelEdit( TableEditable *toCancel) {

    if( table != nullptr && table == toCancel) {
        mode = MODE_RENDER;
    }
}

void SelectList::firstEditableCol( uint8_t row) {

    if( table->isRowEditable( row)) {
        LOGV("SelectList::process(): Is Row Editable (row=%d) => Y\n", row);

        if( table->getColCount( row) > 0) {
            mode = MODE_EDIT;
            tableCol = 0;
            skipNonEditableCol( row);
        }
    }
}

void SelectList::skipNonEditableCol( uint8_t row) {

    for(;;) {
        if( tableCol >= table->getColCount( row)) {
            LOG("SelectList::skipNonEditableCol(): End edit\n");
            mode = MODE_RENDER;
            tableCol = 0;
            break;
    
        } else if( !table->isColEditable( row, tableCol)) {
            tableCol++;
            LOGV("SelectList::skipNonEditableCol(): Next col=%d\n", tableCol);
            continue;

        } else { /* Get next cell to edit */
            table->getValue( row, tableCol, &editCell);
            if( !editCell.isEditable()) {
                tableCol++;
                LOGV("SelectList::skipNonEditableCol(): Next col=%d\n", tableCol);
                continue;
            }
            LOGV("SelectList::skipNonEditableCol(): Edit col=%d\n", tableCol);
            break;
        }
    }
    refresh = REFRESH_UPDATE;  
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
        lcd->headerColors();
        lcd->setCursor( 0, 0);
        lcd->print(header);
    }

    updateTable( lcd);
}

void SelectList::updateTable( LcdWidget *lcd) {

    adjustTopRow( lcd);
    refresh = REFRESH_UPDATE;

    for( uint8_t row = tableTopRow; row < tableTopRow + tableVisibleRows; row++) {        
        refreshLine( lcd, row);
    }

    tableOldRow = tableRow;
}

void SelectList::updateRow( LcdWidget *lcd) {

    if( adjustTopRow( lcd)) {
        updateTable( lcd);

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

    lcd->setCursor( row - tableTopRow + screenHeaderOffs, 0);
    printLine( lcd, row);
}

/* Print a single line on screen.
 * row is the table row number including back item.
 */
void SelectList::printLine( LcdWidget *lcd, uint8_t row) {

    Cell renderCell;
    bool edit;

    (tableRow == row) ? lcd->selectedColors() : lcd->normalColors();

    if( useBackItem) {
        if( row == 0) {
            lcd->print(TEXT_BACK);
            lcd->normalColors();
            lcd->clearEOL();
            return;
        } else {
            row--;
        }
    }

    if( refresh != REFRESH_CELL) {
        lcd->print( table->getRowName(row));
        lcd->normalColors();
        lcd->clearEOL();
    }

    for( uint8_t col = 0; col < table->getColCount(row); col++) {

        edit = (mode == MODE_EDIT && col == tableCol);
        
        if( edit) {
            editCell.render( lcd, edit);
        } else {
            table->getValue( row, col, &renderCell);
            renderCell.render( lcd, edit);
        }
    }
}

