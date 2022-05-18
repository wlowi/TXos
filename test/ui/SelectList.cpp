
#include "SelectList.h"

void SelectList::clear() {

    table = NULL;
    idx = oldIdx = topIdx = 0;
}

void SelectList::set( TableEditable *tab) {

    table = tab;
}

void SelectList::process( LcdWidget *lcd, Event *event) {

    uint8_t refresh = REFRESH_OK;

    switch( event->key) {

        case KEY_ENTER:
            break;

        case KEY_UP:
            prev();
            refresh = REFRESH_UPDATE;
            break;

        case KEY_DOWN:
            next();
            refresh = REFRESH_UPDATE;
            break;
    }

    if( refresh == REFRESH_UPDATE) {
        update( lcd);
        event->markProcessed();
    }
}

void SelectList::prev() {

    if( idx > 0) {
        idx--;
    }
}

void SelectList::next() {

    uint8_t items = table->getItemCount() +1;

    if( idx < items-1) {
        idx++;
    }
}

uint8_t SelectList::current() {

    return idx;
}

void SelectList::paint( LcdWidget *lcd) {

    uint8_t i;
    uint8_t end;
    uint8_t lines = lcd->getLines();
    uint8_t items = table->getItemCount() +1;
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

    for( i = topIdx; i < topIdx + end; i++) {        
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

/* private */

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

    if( i == 0) {
        lcd->print(TEXT_BACK);
    } else {
        lcd->print(table->getItemName(i-1));
    }
}

void SelectList::normalColors( LcdWidget *lcd) {

    lcd->setBg(0,0,0);
    lcd->setFg(255,255,255);
}

void SelectList::selectedColors( LcdWidget *lcd) {

    lcd->setBg(255,255,0);
    lcd->setFg(0,0,0);
}

void SelectList::headerColors( LcdWidget *lcd) {

    lcd->setBg(0,0,0);
    lcd->setFg(0,255,0);
}

