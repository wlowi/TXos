
#include "SelectList.h"

void SelectList::clear() {

    table = nullptr;
    idx = oldIdx = topIdx = 0;
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

    if( table->hasChanged()) {
        refresh = REFRESH_FULL;
    }

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
                if( table->isEditable()) {
                    if( table->getValueCount() > 0) {
                        mode = MODE_EDIT;
                        editCol = 0;
                        table->getValue( tidx, editCol, &cell);
                        refresh = REFRESH_UPDATE;
                    }
                }
                if( table->isExecutable()) {
                    table->execute( tidx);
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
            // ignore
            break;
        }   
        break;

    case MODE_EDIT:

        tidx = useBackItem ? idx-1 : idx;
        cell.edit( event);
        table->setValue( tidx, editCol, &cell);
        refresh = REFRESH_UPDATE;

        if( event->pending()) {
            switch( event->key) {

            case KEY_ENTER:
                editCol++;
                if( editCol >= table->getValueCount()) {
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

    if( refresh == REFRESH_UPDATE) {
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

    uint8_t items = table->getItemCount() + (useBackItem ? 1 : 0);

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

    uint8_t end;
    uint8_t lines = lcd->getLines();
    uint8_t items = table->getItemCount() + (useBackItem ? 1 : 0);
    const char *header = table->getName();

    lcd->setBg(0,0,0);
    lcd->clear();

    if( header) {
        headerColors( lcd);
        lcd->setCursor( 0, 0);
        lcd->print(header);
        lines--;
    }

    adjustTopIdx( lcd);
    end = (items < lines) ? items : lines;

    for( uint8_t i = topIdx; i < topIdx + end; i++) {        
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

    if( table->getName()) {
        lines--;
    }

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

    lcd->print(table->getItemName(i));
    for( uint8_t col = 0; col < table->getValueCount(); col++) {
        table->getValue( i, col, &renderCell);
        normalColors( lcd);
        lcd->print(" ");
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
