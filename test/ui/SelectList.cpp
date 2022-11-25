
#include "SelectList.h"

void SelectList::clear() {

    table = nullptr;
    idx = oldIdx = topIdx = 0;
    visibleRows = 0;
    refresh = REFRESH_FULL;
    mode = MODE_RENDER;
    editCol = 0;
}

void SelectList::set( TableEditable *tab, bool backItem) {

    clear();
    table = tab;
    useBackItem = backItem;
}

void SelectList::process( LcdWidget *lcd, Event *event) {

    uint8_t tidx;

    if( refresh == REFRESH_FULL) {
        paint( lcd);
        refresh = REFRESH_OK;
    }

    switch( mode) {
    case MODE_RENDER:
        switch( event->key) {
        case KEY_ENTER:
            if( !(useBackItem && idx == 0)) {
                tidx = useBackItem ? idx-1 : idx;
                if( table->isRowEditable( tidx)) {
                    LOGV("SelectList::process(): Is Editable (idx=%d)\n", tidx);
                    if( table->getColCount( tidx) > 0) {
                        mode = MODE_EDIT;
                        editCol = 0;
                        table->getValue( tidx, editCol, &cell);
                        refresh = REFRESH_UPDATE;
                    }
                }
                if( table->isRowExecutable( tidx)) {
                    LOGV("SelectList::process(): Is Executable (idx=%d)\n", tidx);
                    table->rowExecute( tidx);
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
            for( tidx = topIdx; tidx < topIdx + visibleRows; tidx++) {
                for( uint8_t col = 0; col < table->getColCount(tidx); col++) {
                    if( table->hasChanged( tidx, col)) {
                        LOGV("P %d %d\n", tidx, col);
                    }        
                }
            }
            break;
        }   
        break;

    case MODE_EDIT:

        tidx = useBackItem ? idx-1 : idx;
        cell.edit( event);
        table->setValue( tidx, editCol, &cell);
        refresh = REFRESH_CELL;

        if( event->pending()) {
            switch( event->key) {

            case KEY_ENTER:
                editCol++;
                if( editCol >= table->getColCount( tidx)) {
                    mode = MODE_RENDER;
                    editCol = 0;
                } else {
                    table->getValue( tidx, editCol, &cell);
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

    if( count >= idx) {
        idx = 0;
    } else {
        idx -= count;
    }
}

void SelectList::next( uint8_t count) {

    uint8_t items = table->getRowCount() + (useBackItem ? 1 : 0);

    if( (idx + count) >= (items - 1) ) {
        idx = items-1;
    } else {
        idx += count;
    }
}

uint8_t SelectList::current() const {

    if( useBackItem) {
        return (idx == 0) ? GO_BACK : idx -1;
    }

    return idx;
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

    adjustTopIdx( lcd);
    
    for( uint8_t i = topIdx; i < topIdx + visibleRows; i++) {        
        refreshLine( lcd, i);
    }

    oldIdx = idx;
}

void SelectList::update( LcdWidget *lcd) {

    if( adjustTopIdx( lcd)) {
        paint( lcd);

    } else {
        refreshLine( lcd, oldIdx);
        refreshLine( lcd, idx);
        oldIdx = idx;
    }
}

bool SelectList::adjustTopIdx( LcdWidget *lcd) {

    uint8_t lines = lcd->getLines();
    uint8_t items = table->getRowCount() + (useBackItem ? 1 : 0);

    if( table->getName()) {
        lines--;
    }

    visibleRows = (items < lines) ? items : lines;

    if( topIdx > idx) {
        topIdx = idx;
    } else if( idx >= (topIdx + lines)) {
        topIdx = idx - (lines -1);
    } else {
        return false;
    }

    return true;
}

void SelectList::refreshLine( LcdWidget *lcd, uint8_t i) {

    uint8_t headerOffs = table->getName() ? 1 : 0;

    (idx == i) ? selectedColors( lcd) : normalColors( lcd);
        
    lcd->setCursor( i - topIdx + headerOffs, 0);
    printItem( lcd, i);
}

void SelectList::printItem( LcdWidget *lcd, uint8_t i) {

    Cell renderCell;

    if( useBackItem) {
        if( i == 0) {
            lcd->print(TEXT_BACK);
            return;
        } else {
            i--;
        }
    }

    if( refresh != REFRESH_CELL) {
        lcd->print(table->getRowName(i));
    }

    for( uint8_t col = 0; col < table->getColCount(i); col++) {
        table->getValue( i, col, &renderCell);
        normalColors( lcd);

        if( mode == MODE_EDIT && col == editCol) {
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
