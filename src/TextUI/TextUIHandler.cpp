/*
  TextUI. A simple text based UI.

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

#include "TextUI.h"

void TextUIHandler::clear(TextUI *ui)
{
    if (screen)
    {
        screen->deactivate(ui);
        screen = nullptr;
    }

    useBackItem = false;
    screenTableRows = 0;
    screenHeaderOffs = 0;
    tableRows = 0;
    tableTopRow = 0;
    tableVisibleRows = 0;
    tableRow = 0;
    tableOldRow = 0;
    tableCol = 0;
    refresh = REFRESH_FULL;
    mode = MODE_RENDER;
}

void TextUIHandler::set(TextUI *ui, TextUIScreen *scr)
{
    UILOG("TextUIHandler::set()\n");

    clear(ui);
    screen = scr;

    if (screen)
    {
        useBackItem = scr->goBackItem();
        tableRows = screen->getRowCount() + (useBackItem ? 1 : 0);
        screenHeaderOffs = screen->getHeader() ? 1 : 0;

        screen->activate(ui);
    }
}

void TextUIHandler::set(TextUI *ui, TextUIScreen *scr, uint8_t currentSelected)
{
    UILOGV("TextUIHandler::set( currentSelected=%d)\n", currentSelected);

    set(ui, scr);

    if( screen->getRowCount() > 0) {
        if (currentSelected >= screen->getRowCount())
        {
            currentSelected = screen->getRowCount() - 1;
        }
        tableRow = useBackItem ? currentSelected + 1 : currentSelected;
    }
}

void TextUIHandler::process(TextUI *ui, TextUILcd *lcd, Event *event)
{
    uint8_t row; // real table row number, excluding back item
    
    /* Let the screen handle events first.
     * The screen may or may not mark the event as "processed".
     */
    screen->handleEvent(ui, event);
    
    /* Screen has no rows and does its own painting,
     * we can quit here.
     */
    if( tableRows == 0) return;

    if (refresh == REFRESH_FULL || refresh == REFRESH_SCREEN)
    {
    	UILOGV("TextUIHandler::process(): FULL REFRESH %d\n", refresh);
        updateScreen(lcd);
    }
    else if (screen->needsRefresh())
    {
	    UILOG("TextUIHandler::process(): TABLE REFRESH requested by screen\n");
        updateTable(lcd);
    }

    if (event->getType() == EVENT_TYPE_KEY)
    {
        UILOGV("TextUIHandler::process(): tableRow=%d\n", tableRow);

        switch (mode)
        {
        case MODE_RENDER:
            switch (event->getKey())
            {
            case KEY_ENTER:
                if (useBackItem && (tableRow == 0))
                {
                    ui->popScreen();
                    event->markProcessed();
                }
                else
                {
                    // tableRow is always > 0
                    row = useBackItem ? tableRow - 1 : tableRow;
                    firstEditableCol(row);

                    if (screen->isRowExecutable(row))
                    {
                        UILOGV("TextUIHandler::process(): Is Row Executable (row=%d) => Y\n", row);
                        screen->rowExecute(ui, row);
                        refresh = REFRESH_ROW;
                    }
                }
                break;

            case KEY_UP:
                if( ui->isReversedNav())
                {
                  prev(event->getCount());
                }
                else
                {
                  next(event->getCount());
                }
                refresh = REFRESH_ROW;
                break;

            case KEY_DOWN:
                if( ui->isReversedNav())
                {
                  next(event->getCount());
                }
                else
                {
                  prev(event->getCount());
                }
                refresh = REFRESH_ROW;
                break;

            default:
                onDemandRefresh(lcd);
                break;
            }
            break;

        case MODE_EDIT:
            editCurrentCell( lcd, event);
            break;

        default:
            // ignore
            break;
        }

        if (refresh == REFRESH_ROW)
        {
            updateRow(lcd);
            event->markProcessed();
            refresh = REFRESH_OK;
        }
    }
    else if ( event->getType() == EVENT_TYPE_TICK)
    { /* Periodic check if a cell was updated in the background */
	    if( mode == MODE_EDIT)
        {    
            editCurrentCell( lcd, event);
            
            if( refresh != REFRESH_OK)
            {
                updateRow(lcd);
                event->markProcessed();
                refresh = REFRESH_OK;
            }
        }
        
        onDemandRefresh(lcd);
    }
}

void TextUIHandler::onDemandRefresh(TextUILcd *lcd)
{
    uint8_t row;

    screen->startRefresh();

    for (uint8_t tRow = tableTopRow; tRow < tableTopRow + tableVisibleRows; tRow++)
    {
        if (useBackItem && tRow == 0)
        {
            continue;
        }

        row = useBackItem ? tRow - 1 : tRow;

        for (uint8_t col = 0; col < screen->getColCount(row); col++)
        {
            if (screen->hasChanged(row, col))
            {
                UILOGV("TextUIHandler::onDemandRefresh(): hasChanged [%d, %d]\n", row, col);
                refreshCell( lcd, tRow, col);
            }
        }
    }

    screen->endRefresh();
}

void TextUIHandler::editCurrentCell( TextUILcd *lcd, Event *event)
{
    uint8_t row = useBackItem ? tableRow - 1 : tableRow;

    if (editCell.edit(event))
    {
        screen->setValue(row, tableCol, &editCell);
        refreshCell( lcd, tableRow, tableCol);
    }
    else
    {
        refresh = REFRESH_OK;
    }

    if (event->getType() == EVENT_TYPE_KEY)
    {
        switch (event->getKey())
        {
        case KEY_ENTER:
            tableCol++;
            skipNonEditableCol(row);
            break;

        default:
            // ignore
            break;
        }
    }
}

void TextUIHandler::cancelEdit(TextUIScreen *toCancel)
{
    if ((screen != nullptr) && (screen == toCancel))
    {
        mode = MODE_RENDER;
    }
}

boolean TextUIHandler::inEditMode()
{
    return mode == MODE_EDIT;
}

void TextUIHandler::firstEditableCol(uint8_t row)
{
    if (screen->isRowEditable(row))
    {
        UILOGV("TextUIHandler::firstEditableCol(): Is Row Editable (row=%d) => Y\n", row);

        if (screen->getColCount(row) > 0)
        {
            mode = MODE_EDIT;
            tableCol = 0;
            skipNonEditableCol(row);
        }
    }
}

void TextUIHandler::skipNonEditableCol(uint8_t row)
{
    for (;;)
    {
        if (tableCol >= screen->getColCount(row))
        {
            UILOG("TextUIHandler::skipNonEditableCol(): End edit\n");
            mode = MODE_RENDER;
            tableCol = 0;
            break;
        }
        else if (!screen->isColEditable(row, tableCol))
        {
            tableCol++;
            UILOGV("TextUIHandler::skipNonEditableCol(): Next col=%d\n", tableCol);
            continue;
        }
        else
        { /* Get next cell to edit */
            editCell.setBlank();
            screen->getValue(row, tableCol, &editCell);
            if (!editCell.isEditable())
            {
                tableCol++;
                UILOGV("TextUIHandler::skipNonEditableCol(): Next col=%d\n", tableCol);
                continue;
            }
            UILOGV("TextUIHandler::skipNonEditableCol(): Edit col=%d\n", tableCol);
            break;
        }
    }
    refresh = REFRESH_ROW;
}

void TextUIHandler::prev(uint8_t count)
{
    if (count >= tableRow)
    {
        tableRow = 0;
    }
    else
    {
        tableRow -= count;
    }
}

void TextUIHandler::next(uint8_t count)
{
    if ((tableRow + count) >= (tableRows - 1))
    {
        tableRow = tableRows - 1;
    }
    else
    {
        tableRow += count;
    }
}

/* private */

void TextUIHandler::updateScreen(TextUILcd *lcd)
{
    UILOG("TextUIHandler::updateScreen():\n");

    const char *header = screen->getHeader();

    if (header)
    {
        lcd->selectedColors();
        lcd->setCursor(0, 0);
        lcd->printStr(header, lcd->getColumns());
        lcd->normalColors();
        lcd->clearEOL();
    }

    updateTable(lcd);
}

void TextUIHandler::updateTable(TextUILcd *lcd)
{
    UILOG("TextUIHandler::updateTable():\n");

    screen->startRefresh();

    useBackItem = screen->goBackItem();
    tableRows = screen->getRowCount() + (useBackItem ? 1 : 0);
    screenHeaderOffs = screen->getHeader() ? 1 : 0;

    if( tableTopRow > tableRows-1) {
        tableTopRow = tableRows-1;
    }

    if( tableRow > tableRows-1) {
        tableRow = tableRows-1;
    }

    UILOGV("TextUIHandler::updateTable(): rows %d topRow %d row%d\n", tableRows, tableTopRow, tableRow);

    adjustTopRow(lcd);
    refresh = REFRESH_ROW;

    for (uint8_t row = tableTopRow; row < tableTopRow + screenTableRows; row++)
    {
        refreshLine(lcd, row);
    }

    tableOldRow = tableRow;
    refresh = REFRESH_OK;

    screen->endRefresh();
}

void TextUIHandler::updateRow(TextUILcd *lcd)
{
    if (adjustTopRow(lcd))
    {
        updateTable(lcd);
    }
    else
    {
        refreshLine(lcd, tableOldRow);
        refreshLine(lcd, tableRow);
        tableOldRow = tableRow;
    }
}

/* Recomputes
 *  screenTableRows
 *  tableTopRow
 *  tableVisibleRows
 */
bool TextUIHandler::adjustTopRow(TextUILcd *lcd)
{
    screenTableRows = lcd->getRows() - screenHeaderOffs;
    tableVisibleRows = (tableRows < screenTableRows) ? tableRows : screenTableRows;

    if (tableRow < tableTopRow)
    {
        tableTopRow = tableRow;
    }
    else if (tableRow >= (tableTopRow + screenTableRows))
    {
        tableTopRow = tableRow - (screenTableRows - 1);
    }
    else
    {
        return false;
    }

    return true;
}

void TextUIHandler::refreshLine(TextUILcd *lcd, uint8_t row)
{
    uint8_t renderRow;
    
    UILOGV("TextUIHandler::refreshLine(): row=%d tableRows=%d refr=%d\n", row, tableRows, refresh);

    lcd->setCursor(row - tableTopRow + screenHeaderOffs, 0);

    if (row >= tableRows)
    {
        lcd->normalColors();
        lcd->clearEOL();
        return;
    }

    (tableRow == row) ? lcd->selectedColors() : lcd->normalColors();

    if (useBackItem)
    {
        if (row == 0)
        {
            lcd->printStr(TEXTUI_TEXT_BACK);
            lcd->normalColors();
            lcd->clearEOL();
            return;
        }
        else
        {
            renderRow = row -1;
        }
    }
    else
    {
        renderRow = row;
    }

    lcd->printStr(screen->getRowName(renderRow));
    lcd->normalColors();
    lcd->clearEOL();

    for (uint8_t col = 0; col < screen->getColCount(renderRow); col++)
    {
	    refreshCell( lcd, row, col);
    }
}

void TextUIHandler::refreshCell( TextUILcd *lcd, uint8_t row, uint8_t col)
{
    Cell renderCell;
    bool edit = (mode == MODE_EDIT && row == tableRow && col == tableCol);

    UILOGV("TextUIHandler::refreshCell(): edit=%d [%d, %d] [%d, %d]\n", edit, tableRow, tableCol, row, col);
    
    lcd->setRow( row - tableTopRow + screenHeaderOffs);
        
    if (edit)
    {
        editCell.render( lcd, edit);
    }
    else
    {
        screen->getValue( useBackItem ? row-1: row, col, &renderCell);
        renderCell.render( lcd, edit);
    }
}

