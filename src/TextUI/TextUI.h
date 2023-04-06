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

#ifndef _TextUI_h_
#define _TextUI_h_

#include "Arduino.h"

/* EXTENSION FOR TXOS */
#include "Controls.h"

// #define TEXTUI_DEBUG

#ifdef TEXTUI_DEBUG
    #if defined(ARDUINO)
        #define UILOG(f)
        #define UILOGV(f, ...)
    #else
        #include "stdio.h"
        #include <cstring>
        #define UILOG(f) printf(f)
        #define UILOGV(f, ...) printf(f, __VA_ARGS__)
    #endif
#else
    #define UILOG(f)
    #define UILOGV(f, ...)
#endif

#define EVENT_TYPE_NONE 0
#define EVENT_TYPE_IRQ 1
#define EVENT_TYPE_TIMER 2
#define EVENT_TYPE_KEY 3

#define KEY_NONE  0
#define KEY_UP    1
#define KEY_ENTER 2
#define KEY_DOWN  3
#define KEY_BACK  4
#define KEY_CLEAR 5
#define KEY_RESET 6

#define TEXTUI_FONT_SMALL    0
#define TEXTUI_FONT_MEDIUM   1
#define TEXTUI_FONT_LARGE    2

#define TEXTUI_TEXT_ON_OFF_length 3
#define TEXTUI_TEXT_ON "On"
#define TEXTUI_TEXT_OFF "Off"

#define TEXTUI_TEXT_BACK "<<<<"

#define REFRESH_OK     0
#define REFRESH_FULL   1
#define REFRESH_SCREEN 2
#define REFRESH_ROW    3
#define REFRESH_CELL   4

#define MODE_RENDER 0
#define MODE_EDIT 1

#define TEXTUI_SCREENSTACK_SZ 5

typedef uint16_t uiTimer_t;

typedef long fixfloat1_t;
typedef long fixfloat2_t;

class Event
{

private:
    uint8_t eventType = EVENT_TYPE_NONE;
    uint8_t key = KEY_NONE;
    uint8_t count = 0;
    bool eventPending = false;

public:
    void setNoEvent()
    {
        eventType = EVENT_TYPE_NONE;
        key = KEY_NONE;
        count = 0;
        eventPending = false;
    }

    void setIRQEvent()
    {
        eventType = EVENT_TYPE_IRQ;
        key = KEY_NONE;
        count = 0;
        eventPending = true;
    }

    void setTimerEvent()
    {
        eventType = EVENT_TYPE_TIMER;
        key = KEY_NONE;
        count = 0;
        eventPending = true;
    }

    void setKeyEvent(uint8_t k, uint8_t c)
    {
        eventType = EVENT_TYPE_KEY;
        key = k;
        count = c;
        eventPending = true;
    }

    uint8_t getType() { return eventType; }
    uint8_t getCount() { return count; }
    uint8_t getKey() { return key; }

    void markProcessed() { setNoEvent(); }
    bool pending() const { return eventPending; }
};

/* Base class for all input devices.
 * Buttons, Keyboard, Rotary Encoder ...
 */
class TextUIInput
{

private:
    TextUIInput *next = nullptr;

public:
    TextUIInput() {}

    void append(TextUIInput *in)
    {
        if (next != nullptr)
        {
            next->append(in);
        }
        else
        {
            next = in;
        }
    }

    TextUIInput *getNext()
    {
        return next;
    }

    /* Returns true if there is an event pending.
     * setEvent() clears pending state.
     * 
     * Note that there may be multiple events pending
     * for different keys. setEvent() clears the pending
     * state only for the key returned.
     */
    virtual bool pending() = 0;
    virtual void setEvent(Event *e) = 0;
};

/* Base class for all LCD displays.
 */
class TextUILcd
{

private:
    void printLongGeneric(unsigned long val, int8_t neg, uint8_t width, uint8_t dot, char filler);

public:
    TextUILcd() {}

    virtual void clear() = 0;
    virtual void clearEOL() = 0;

    virtual bool colorSupport() = 0;

    virtual void setBg(uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void setFg(uint8_t r, uint8_t g, uint8_t b) = 0;

    virtual void normalColors() = 0;
    virtual void selectedColors() = 0;

    virtual void setInvert(bool inv) = 0;

    virtual void setFontSize(uint8_t sz) = 0;

    virtual uint16_t getRows() = 0;
    virtual uint16_t getColumns() = 0;

    virtual void setCursor(uint8_t r, uint8_t c) = 0;
    virtual void setRow(uint8_t r) = 0;
    virtual void setColumn(uint8_t c) = 0;

    virtual void printChar(char ch) = 0;

    void printInt(int val);
    void printInt(int val, uint8_t width);
    void printInt(int val, uint8_t width, char filler);

    void printUInt(unsigned int val);
    void printUInt(unsigned int val, uint8_t width);
    void printUInt(unsigned int val, uint8_t width, char filler);

    void printLong(long val);
    void printLong(long val, uint8_t width);
    void printLong(long val, uint8_t width, char filler);

    void printULong(unsigned long val);
    void printULong(unsigned long val, uint8_t width);
    void printULong(unsigned long val, uint8_t width, char filler);

    void printStr(const char str[]);
    void printStr(const char str[], uint8_t width);
    void printStr(const char str[], uint8_t width, int8_t editIdx);

#ifdef ARDUINO
    void printStr(const __FlashStringHelper *str);
    void printStr(const __FlashStringHelper *str, uint8_t width);
#endif

    void printFixFloat1(fixfloat1_t val, uint8_t width);
    void printFixFloat2(fixfloat2_t val, uint8_t width);
};

/* NOTE: 
 * If you add a new type please update Cell::isEditable()
 */
enum CellEditType_t
{
    BLANK_T,
    BOOLEAN_T,
    INT8_T,
    INT16_T,
    INT32_T,
    FLOAT1,
    FLOAT2,
    STRING_T,
    LABEL_T,
    FLABEL_T,
    LIST_T,

    /* EXTENSION FOR TXOS */
    SWITCH_T,
    SWITCH_SET_STATE_T
};

typedef struct CellType_t
{

    bool boolV;

    int32_t intV;

    uint8_t size; /* String length */
    char *string;
    const char *label;
    const __FlashStringHelper *flabel;
    
    uint8_t count; /* List size */
    const char *const *list;

} CellType_t;

class Cell
{

private:
    CellEditType_t type = BLANK_T;
    CellType_t value;
    int32_t numericMin;
    int32_t numericMax;

    uint8_t screenCol;

public:
    void setBlank();
    void setBool(uint8_t screenX, bool v);
    void setInt8(uint8_t screenX, int8_t v, uint8_t width, int16_t nmin, int16_t nmax);
    void setInt16(uint8_t screenX, int16_t v, uint8_t width, int16_t nmin, int16_t nmax);
    void setInt32(uint8_t screenX, int32_t v, uint8_t width, int32_t nmin, int32_t nmax);
    void setFloat1(uint8_t screenX, fixfloat1_t v, uint8_t width, fixfloat1_t nmin, fixfloat1_t nmax);
    void setFloat2(uint8_t screenX, fixfloat2_t v, uint8_t width, fixfloat2_t nmin, fixfloat2_t nmax);
    void setString(uint8_t screenX, char *v, uint8_t sz);
    void setLabel(uint8_t screenX, const char *v, uint8_t sz);
#ifdef ARDUINO
    void setLabel(uint8_t screenX, const __FlashStringHelper *v, uint8_t sz);
#endif
    void setList(uint8_t screenX, const char *const *v, uint8_t sz, uint8_t curr);

    /* EXTENSION FOR TXOS */
    void setSwitch( uint8_t screenX, switch_t v);
    void setSwitchState( uint8_t screenX, switch_t v);

    bool getBool() const;
    int8_t getInt8() const;
    int16_t getInt16() const;
    int32_t getInt32() const;
    fixfloat1_t getFloat1() const;
    fixfloat2_t getFloat2() const;
    char *getString() const;
    uint8_t getList() const;

    /* EXTENSION FOR TXOS */
    switch_t getSwitch() const;
    switch_t getSwitchState() const;

    void render(TextUILcd *lcd, bool edit);
    /* returns true if cell has been changed. */
    bool edit(Event *event);
    bool isEditable();
};

class TextUI;

class TextUIMenu;

class TextUIScreen
{
    friend class TextUIMenu;

protected:
    TextUIScreen *menuNext;
    /* Last selected item (row number) */
    uint8_t selection = 0;

public:
    /* This is the name of the module if it appears within a menu */
    virtual const char *getHeader() = 0;
    virtual const char *getMenuName() = 0;

    void setSelection( uint8_t sel) { selection = sel; }
    uint8_t getSelection() { return selection; }

    virtual bool goBackItem() { return false; }

    /* Called whenever a different screen is activated before calling process() for the first time */
    virtual void activate(TextUI *ui)
    { /* default implementation does nothing */
    }
    virtual void deactivate(TextUI *ui)
    { /* default implementation does nothing */
    }

    /* Process key and other events.
     *
     * If the event was handled
     * e->markProcessed() should be called to prevent the UI from also handle the same event again.
     */
    virtual void handleEvent(TextUI *ui, Event *e)
    { /* default implementation does nothing */
    }

    /* Return true if values of the row are editable */
    virtual bool isRowEditable(uint8_t row) { return true; }
    /* Return true if the value of this cell is editable */
    virtual bool isColEditable(uint8_t row, uint8_t col) { return false; }

    /* Return true if rowExecute should be called
     * when the row is activated.
     */
    virtual bool isRowExecutable(uint8_t row) { return false; }
    virtual void rowExecute(TextUI *ui, uint8_t row)
    { /* default implementation does nothing */
    }

    /* Check whether the module itself is executable.
     * This will call moduleExecute() of the module if it is
     * activated within a menu.
     */
    virtual bool isModuleExecutable() { return false; }
    /* Called when isModuleExecute() returns true and the user presses the ENTER key */
    virtual void moduleExecute(TextUI *ui)
    { /* default implementation does nothing */
    }

    virtual uint8_t getRowCount() = 0;
    virtual const char *getRowName(uint8_t row) = 0;

    virtual uint8_t getColCount(uint8_t row) = 0;

    /* Called before and after table redraw */
    virtual void startRefresh() {/* default implementation does nothing */};
    virtual void endRefresh() {/* default implementation does nothing */};

    /* Table requests full redraw */
    virtual bool needsRefresh() { return false; };

    /* Returns true if a value has changed without user interaction */
    virtual bool hasChanged(uint8_t row, uint8_t col) { return false; }
    virtual void getValue(uint8_t row, uint8_t col, Cell *cell) = 0;
    virtual void setValue(uint8_t row, uint8_t col, Cell *cell) {/* default implementation does nothing */};
};

class TextUIMenu : public TextUIScreen
{

private:
    const char *header;
    bool useGoBackItem = false;
    TextUIScreen *first = nullptr;
    TextUIScreen *last = nullptr;

public:
    TextUIMenu(const char *hdr);
    TextUIMenu(const char *hdr, bool goBackItem);

    void addScreen(TextUIScreen *screenPtr);

    uint8_t getScreenCount();
    TextUIScreen *getFirstScreen();
    TextUIScreen *getNextScreen( TextUIScreen *scr);
    TextUIScreen *getScreen(uint8_t idx);

    /* From TextUIScreen */
    const char *getHeader();
    const char *getMenuName();

    bool goBackItem() { return useGoBackItem; }

    bool isRowExecutable(uint8_t row);
    void rowExecute(TextUI *ui, uint8_t row);

    uint8_t getRowCount();
    const char *getRowName(uint8_t row);

    uint8_t getColCount(uint8_t row);
    void getValue(uint8_t row, uint8_t col, Cell *cell);
};

class TextUIHandler
{

private:
    TextUIScreen *screen = nullptr;

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

    uint8_t tableRow; /* current table row, including back item*/
    uint8_t tableOldRow;
    uint8_t tableCol; /* current table col */

    uint8_t refresh;
    uint8_t mode;

    void editCurrentCell(Event *event);
    void onDemandRefresh(TextUILcd *lcd);

    bool adjustTopRow(TextUILcd *lcd);
    void refreshLine(TextUILcd *lcd, uint8_t i);
    void printLine(TextUILcd *lcd, uint8_t i);
    void updateScreen(TextUILcd *lcd);
    void updateTable(TextUILcd *lcd);
    void updateRow(TextUILcd *lcd);
    void firstEditableCol(uint8_t row);
    void skipNonEditableCol(uint8_t row);
    void prev(uint8_t count);
    void next(uint8_t count);
    void clear(TextUI *ui);

public:
    void set(TextUI *ui, TextUIScreen *scr);
    void set(TextUI *ui, TextUIScreen *scr, uint8_t currentSelected);
    void process(TextUI *ui, TextUILcd *lcd, Event *event);
    /* Cancel edit for this table */
    void cancelEdit(TextUIScreen *toCancel);
    boolean inEditMode();
    void forceRefresh() { refresh = REFRESH_SCREEN; }
    uint8_t current() const;
};

class TextUI
{

private:
    Event event;
    TextUIHandler handler;

    TextUIScreen *homeScreen = nullptr;

    TextUIScreen *screenStack[TEXTUI_SCREENSTACK_SZ];
    uint8_t stackPtr = 0;
#define CURRENT_SCREEN (screenStack[stackPtr])

    uint8_t refresh = REFRESH_FULL;
    boolean reversedNav = false;
    boolean itemPopped = false;
    TextUILcd *display = nullptr;
    TextUIInput *inputQueue = nullptr;
    uiTimer_t timer_msec = 0;
    unsigned long nextTimer_msec = 0;

public:
    void setTimer(uiTimer_t msec);
    void setDisplay(TextUILcd *lcd);
    TextUILcd *getDisplay() { return display; }
    void setInput(TextUIInput *in);
    void setHomeScreen(TextUIScreen *scr);
    /* Reverse UI navigation. 
     * Up key = down and vice versa.
     */
    void setReversedNav( boolean v);
    boolean isReversedNav();

    boolean inEditMode();
    /* Cancel edit for this table */
    void cancelEdit(TextUIScreen *toCancel);
    
    Event *getEvent();
    void handle(Event *ev);

    void forceRefresh() { refresh = REFRESH_SCREEN; }
    
    void toHome();
    void toScreen(TextUIScreen *scr);
    void switchScreen(TextUIScreen *scr);
    void pushScreen(TextUIScreen *scr);
    void popScreen();
};

#endif
