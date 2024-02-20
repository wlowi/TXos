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

#define TEXTUI_DEBUG

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

enum EventType_t {

    EVENT_TYPE_NONE,
    EVENT_TYPE_TICK,
    EVENT_TYPE_TIMER,
    EVENT_TYPE_KEY
};

#define KEY_NONE       0
#define KEY_UP         1
#define KEY_ENTER      2
#define KEY_DOWN       3
#define KEY_BACK       4
#define KEY_CLEAR      5
#define KEY_RESET      6
#define KEY_LEFT       7
#define KEY_RIGHT      8
#define KEY_NUMBER     9
#define KEY_ALPHA     10
#define KEY_FUNCTION  11

enum FontSize_t {

    TEXTUI_FONT_SMALL,
    TEXTUI_FONT_MEDIUM,
    TEXTUI_FONT_LARGE
};

static const uint8_t TEXTUI_TEXT_ON_OFF_length = 3;
#define TEXTUI_TEXT_ON   F("On")
#define TEXTUI_TEXT_OFF  F("Off")
#define TEXTUI_TEXT_BACK F("<<<<")

enum Refresh_t {

    REFRESH_OK,
    REFRESH_FULL,
    REFRESH_SCREEN,
    REFRESH_SCREEN_SET_ROW,
    REFRESH_ROW
};

enum Mode_t {

    MODE_RENDER,
    MODE_EDIT
};


typedef uint16_t uiTimer_t;

typedef long fixfloat1_t;
typedef long fixfloat2_t;


static const uiTimer_t EVENT_TICK_msec = 100;
static const uint8_t TEXTUI_SCREENSTACK_SZ = 5;


class TextUI;

/**
 * @brief The event class.
 * 
 */
class Event
{

private:
    EventType_t eventType = EVENT_TYPE_NONE;
    uint8_t key = KEY_NONE;
    uint8_t count = 0;
    bool eventPending = false;

public:
    /**
     * @brief Set event type to 'No Event'.
     */
    void setNoEvent()
    {
        eventType = EVENT_TYPE_NONE;
        key = KEY_NONE;
        count = 0;
        eventPending = false;
    }

    /**
     * @brief Set event type to 'Tick Event'.
     */
    void setTickEvent()
    {
        eventType = EVENT_TYPE_TICK;
        key = KEY_NONE;
        count = 0;
        eventPending = true;
    }

    /**
     * @brief Set event type to 'Timer Event'.
     */
    void setTimerEvent()
    {
        eventType = EVENT_TYPE_TIMER;
        key = KEY_NONE;
        count = 0;
        eventPending = true;
    }

    /**
     * @brief Set event type to 'Key Event'.
     * 
     * @param k uint8_t: Key
     * @param c uint8_t: Repeat count.
     */
    void setKeyEvent(uint8_t k, uint8_t c)
    {
        eventType = EVENT_TYPE_KEY;
        key = k;
        count = c;
        eventPending = true;
    }

    /**
     * @brief Get the event type.
     * 
     * @return EventType_t: Event type 
     */
    EventType_t getType() { return eventType; }

    /**
     * @brief Get the event repeat count:
     * 
     * @return uint8_t: Repeat count
     */
    uint8_t getCount() { return count; }

    /**
     * @brief Get the event key.
     * 
     * @return uint8_t: Event key
     */
    uint8_t getKey() { return key; }

    /**
     * @brief Mark event as processed.
     * 
     * Marking an event as 'processed' changes the event type to EVENT_TYPE_NONE.
     */
    void markProcessed() { setNoEvent(); }

    /**
     * @brief Check for unprocessed (pending) event.
     * 
     * @return bool: 'true' if the event is still pending to get processed.
     */
    bool pending() const { return eventPending; }
};

/**
 * @brief Base class for all input devices.
 * 
 * Buttons, Keyboard, Rotary Encoder ...
 */
class TextUIInput
{
    friend class TextUI;

private:
    TextUIInput *next = nullptr;

    /**
     * @brief Get the Next object
     * 
     * @return TextUIInput* 
     */
    TextUIInput *getNext()
    {
        return next;
    }

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

public:
    TextUIInput() {}

    /**
     * @brief Check for pending event.
     *
     * Note that there may be multiple events pending
     * for different keys. setEvent() clears the pending
     * state only for the key returned.
     * 
     * setEvent() clears the pending state.
     * 
     * @returns boolean: 'true' if there is an event pending.
     */
    virtual bool pending() = 0;

    /**
     * @brief Set event if there is an event pending.
     * 
     * @param e 
     */
    virtual void setEvent(Event *e) = 0;
};

/**
 * @brief Base class for all LCD displays.
 */
class TextUILcd
{

private:
    void printLongGeneric(unsigned long val, int8_t neg, uint8_t width, uint8_t dot, char filler);

public:
    /**
     * @brief Construct a new TextUILcd.
     * 
     */
    TextUILcd() {}

    /**
     * @brief Clear the display.
     */
    virtual void clear() = 0;
    /**
     * @brief Clear current line to end of line.
     */
    virtual void clearEOL() = 0;

    /**
     * @brief Check for color support.
     * 
     * @return bool: 'true' if the display supports colors.
     */
    virtual bool colorSupport() = 0;

    /**
     * @brief Set background color.
     * 
     * @param r uint8_t: Red component
     * @param g uint8_t: Green component
     * @param b uint8_t: Blue component
     */
    virtual void setBg(uint8_t r, uint8_t g, uint8_t b) = 0;
    /**
     * @brief Set foreground color.
     * 
     * @param r uint8_t: Red component 
     * @param g uint8_t: Green component
     * @param b uint8_t: Blue component
     */
    virtual void setFg(uint8_t r, uint8_t g, uint8_t b) = 0;

    /**
     * @brief Switch to predefined set of colors.
     */
    virtual void normalColors() = 0;
    /**
     * @brief Switch to predefined set of colors for selected items.
     */
    virtual void selectedColors() = 0;
    /**
     * @brief Switch to predefined set of colors for items that are currently edited.
     */
    virtual void editColors() = 0;

    /**
     * @brief Enable/disable inverted mode.
     * 
     * @param inv bool: 'true' switch to inverted colors.
     */
    virtual void setInvert(bool inv) = 0;

    /**
     * @brief Set the font size.
     * 
     * @param sz FontSize_t
     */
    virtual void setFontSize(FontSize_t sz) = 0;

    /**
     * @brief Get number of text rows.
     * 
     * @return uint8_t 
     */
    virtual uint8_t getRows() = 0;
    /**
     * @brief Get number of text columns.
     * 
     * @return uint8_t 
     */
    virtual uint8_t getColumns() = 0;

    /**
     * @brief Set the text cursor.
     * 
     * Row and column start at 0.
     * 
     * @param r uint8_t: Text row.
     * @param c uint8_t: Text column.
     */
    virtual void setCursor(uint8_t r, uint8_t c) = 0;
    /**
     * @brief Set the text row.
     * 
     * @param r uint8_t: Text row.
     */
    virtual void setRow(uint8_t r) = 0;
    /**
     * @brief Set the text column.
     * 
     * @param c uint8_t: Text column.
     */
    virtual void setColumn(uint8_t c) = 0;

    /**
     * @brief Print a single character.
     * 
     * @param ch char: Character to print.
     */
    virtual void printChar(char ch) = 0;

    /**
     * @brief Print signed integer.
     * 
     * Variable field width.
     * 
     * @param val int: Value to print
     */
    void printInt(int val);
    /**
     * @brief Print signed integer.
     * 
     * Fixed field width, right justified.
     * 
     * @param val int: Value to print
     * @param width uint8_t: Field width
     */
    void printInt(int val, uint8_t width);
    /**
     * @brief Print signed integer.
     * 
     * Fixed field width, right justified, with filler character.
     * 
     * @param val int: Value to print
     * @param width uint8_t: Field width
     * @param filler char: Filler character
     */
    void printInt(int val, uint8_t width, char filler);

    /**
     * @brief Print unsigned integer.
     * 
     * Variable field width.
     * 
     * @param val unsigned int: Value to print
     */
    void printUInt(unsigned int val);
    /**
     * @brief Print unsigned integer.
     * 
     * Fixed field width, right justified.
     * 
     * @param val unsigned int: Value to print
     * @param width uint8_t: Field width
     */
    void printUInt(unsigned int val, uint8_t width);
    /**
     * @brief Print unsigned integer.
     * 
     * Fixed field width, right justified, with filler character.
     * 
     * @param val unsigned int: Value to print
     * @param width uint8_t: Field width
     * @param filler char: Filler character
     */
    void printUInt(unsigned int val, uint8_t width, char filler);

    /**
     * @brief Print signed long.
     * 
     * Variable field width.
     * 
     * @param val long: Value to print
     */
    void printLong(long val);
    /**
     * @brief Print signed long.
     * 
     * Fixed field width, right justified.
     * 
     * @param val long: Value to print
     * @param width uint8_t: Field width
     */
    void printLong(long val, uint8_t width);
    /**
     * @brief Print signed long.
     * 
     * Fixed field width, right justified, with filler character.
     * 
     * @param val long: Value to print
     * @param width uint8_t: Field width
     * @param filler char: Filler character
     */
    void printLong(long val, uint8_t width, char filler);

    /**
     * @brief Print unsigned long.
     * 
     * Variable field width.
     * 
     * @param val unsigned long: Value to print
     */
    void printULong(unsigned long val);
    /**
     * @brief Print unsigned long.
     * 
     * Fixed field width, right justified.
     * 
     * @param val unsigned long: Value to print
     * @param width uint8_t: Field width
     */
    void printULong(unsigned long val, uint8_t width);
    /**
     * @brief Print unsigned long.
     * 
     * Fixed field width, right justified, with filler character.
     * 
     * @param val unsigned long: Value to print
     * @param width uint8_t: Field width
     * @param filler char: Filler character
     */
    void printULong(unsigned long val, uint8_t width, char filler);

    /**
     * @brief Print character string.
     * 
     * Variable field width.
     * 
     * @param str const char str[]: String to print
     */
    void printStr(const char str[]);
    /**
     * @brief Print character string.
     * 
     * Fixed field width, left justified.
     * 
     * @param str const char str[]: String to print
     * @param width uint8_t: Field width
     */
    void printStr(const char str[], uint8_t width);
    /**
     * @brief Print character string.
     * 
     * Fixed field width, left justified.
     * Mark character at index 'editIdx'.
     * editIdx is ignored if < 0.
     * 
     * @param str const char str[]: String to print
     * @param width uint8_t: Field width
     * @param editIdx int8_t: Mark character at index
     */
    void printStr(const char str[], uint8_t width, int8_t editIdx);

#ifdef ARDUINO
    /**
     * @brief Print character string from flash memory.
     * 
     * Variable field width.
     * 
     * @param str const __FlashStringHelper *: String to print
     */
    void printStr(const __FlashStringHelper *str);
    /**
     * @brief Print character string from flash memory.
     * 
     * Fixed field width, left justified.
     * 
     * @param str const __FlashStringHelper *: String to print
     * @param width uint8_t: Field width
     */
    void printStr(const __FlashStringHelper *str, uint8_t width);
#endif

    /**
     * @brief Print fixed float.
     * 
     * Print a number with 1 fractional digit.
     * 
     * @param val fixfloat1_t: the number to print
     * @param width uint8_t: Field width
     */
    void printFixFloat1(fixfloat1_t val, uint8_t width);
    /**
     * @brief Print fixed float.
     * 
     * Print a number with 2 fractional digits.
     * 
     * @param val fixfloat2_t: The number to print
     * @param width uint8_t: Field width
     */
    void printFixFloat2(fixfloat2_t val, uint8_t width);
};

/**
 * @brief Cell edit type
 * 
 * NOTE: If you add a new type please update Cell::isEditable()
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

/**
 * @brief Holds various cell values.
 */
typedef struct CellValue_t
{
    bool boolV; /**< A boolean value. */

    int32_t intV; /**< An integer value. */

    uint8_t size; /**< String length. */
    char *string; /**< A character string. */
    const char *label; /**< A label. Not editable. */
    const __FlashStringHelper *flabel; /**< A label in flash memory. Not editable. */
    
    uint8_t count; /**< List size. */
    const char *const *list; /**< A list. */

} CellValue_t;

/**
 * @brief A class to hold editable values of various types.
 * 
 */
class Cell
{

private:
    CellEditType_t type = BLANK_T;
    CellValue_t value;
    int32_t numericMin;
    int32_t numericMax;

    uint8_t screenCol;

public:
    /**
     * @brief Set the cell to a blank value.
     */
    void setBlank();
    /**
     * @brief Set the cell to a boolean value.
     * 
     * @param screenX uint8_t: Screen column
     * @param v bool: The boolean value
     */
    void setBool(uint8_t screenX, bool v);
    /**
     * @brief Set the cell to an integer number.
     * 
     * @param screenX uint8_t: Screen column
     * @param v int8_t: The integer number
     * @param width uint8_t: Field width
     * @param nmin int16_t: Minimum value for editing
     * @param nmax int16_t: Maximum value for editing
     */
    void setInt8(uint8_t screenX, int8_t v, uint8_t width, int16_t nmin, int16_t nmax);
    /**
     * @brief Set the cell to an integer number.
     * 
     * @param screenX uint8_t: Screen column
     * @param v int16_t: The integer number
     * @param width uint8_t: Field width
     * @param nmin int16_t: Minimum value for editing
     * @param nmax int16_t: Maximum value for editing
     */
    void setInt16(uint8_t screenX, int16_t v, uint8_t width, int16_t nmin, int16_t nmax);
    /**
     * @brief Set the cell to an integer number.
     * 
     * @param screenX uint8_t: Screen column
     * @param v int32_t: The integer number
     * @param width uint8_t: Field width
     * @param nmin int32_t: Minimum value for editing
     * @param nmax int32_t: Maximum value for editing
     */
    void setInt32(uint8_t screenX, int32_t v, uint8_t width, int32_t nmin, int32_t nmax);
    /**
     * @brief Set the cell to a fixed float number.
     * 
     * @param screenX uint8_t: Screen column
     * @param v fixfloat1_t: The integer number
     * @param width uint8_t: Field width
     * @param nmin fixfloat1_t: Minimum value for editing
     * @param nmax fixfloat1_t: Maximum value for editing
     */
    void setFloat1(uint8_t screenX, fixfloat1_t v, uint8_t width, fixfloat1_t nmin, fixfloat1_t nmax);
    /**
     * @brief Set the cell to a fixed float number.
     * 
     * @param screenX uint8_t: Screen column
     * @param v fixfloat2_t: The integer number
     * @param width uint8_t: Field width
     * @param nmin fixfloat2_t: Minimum value for editing
     * @param nmax fixfloat2_t: Maximum value for editing
     */
    void setFloat2(uint8_t screenX, fixfloat2_t v, uint8_t width, fixfloat2_t nmin, fixfloat2_t nmax);

    /**
     * @brief Set the cell to a character string.
     * 
     * @param screenX uint8_t: Screen column
     * @param v char *: Pointer to string buffer
     * @param sz uint8_t: String buffer size
     */
    void setString(uint8_t screenX, char *v, uint8_t sz);
    /**
     * @brief Set the cell to a character string label.
     * 
     * A label is not editable
     * 
     * @param screenX uint8_t: Screen column
     * @param v const char *: Pointer to string buffer
     * @param sz uint8_t: String buffer size
     */
    void setLabel(uint8_t screenX, const char *v, uint8_t sz);
#ifdef ARDUINO
    /**
     * @brief Set the cell to a character string label in flash memory.
     * 
     * A label is not editable
     * 
     * @param screenX uint8_t: Screen column
     * @param v const __FlashStringHelper *: Pointer to string buffer
     * @param sz uint8_t: String buffer size
     */
    void setLabel(uint8_t screenX, const __FlashStringHelper *v, uint8_t sz);
#endif
    /**
     * @brief Set the cell to a list of strings.
     * 
     * @param screenX uint8_t: Screen column
     * @param v const char *const *: An array of character strings
     * @param sz uint8_t: Number of elements in the array
     * @param curr uint8_t: Currently selected array element
     */
    void setList(uint8_t screenX, const char *const *v, uint8_t sz, uint8_t curr);

    /* EXTENSION FOR TXOS */
    void setSwitch( uint8_t screenX, switch_t v);
    void setSwitchState( uint8_t screenX, switch_t v);

    /**
     * @brief Get the boolean value of the cell.
     * 
     * @return bool:
     */
    bool getBool() const;
    /**
     * @brief Get the integer number of a cell.
     * 
     * @return int8_t 
     */
    int8_t getInt8() const;
    /**
     * @brief Get the integer number of a cell.
     * 
     * @return int16_t 
     */
    int16_t getInt16() const;
    /**
     * @brief Get the integer number of a cell.
     * 
     * @return int32_t 
     */
    int32_t getInt32() const;
    /**
     * @brief Get the fixed float number of a cell.
     * 
     * @return fixfloat1_t
     */
    fixfloat1_t getFloat1() const;
    /**
     * @brief Get the fixed float number of a cell.
     * 
     * @return fixfloat2_t
     */
    fixfloat2_t getFloat2() const;

    /**
     * @brief Get the string value of a cell.
     * 
     * This function is not really necessary as a string in always edited in place.
     * The returned value is the same as previously set with setString().
     * 
     * @return char*: A pointer to a string
     */
    char *getString() const;

    /**
     * @brief Get the index of the selected list element.
     * 
     * @return uint8_t: The index of the currently selected list element. 
     */
    uint8_t getList() const;

    /* EXTENSION FOR TXOS */
    switch_t getSwitch() const;
    switch_t getSwitchState() const;

    /**
     * @brief Render the cell value to a display
     * 
     * @param lcd TextUILcd*: A display
     * @param edit bool: 'true' if the cell should be in edit mode
     */
    void render(TextUILcd *lcd, bool edit);

    /**
     * @brief Edit a cell.
     * 
     * Editing usually means changing the cell value.
     * 
     * @param event Event*: Usually a key event.
     * @return bool: true if cell value has been changed
     */
    bool edit(Event *event);

    /**
     * @brief Check if the cell is editable.
     * 
     * @return bool: 'true' if the cell is editable
     */
    bool isEditable();
};

class TextUIMenu;

/**
 * @brief Base class for all screens.
 * 
 */
class TextUIScreen
{
    friend class TextUIMenu;

private:
    TextUIScreen *menuNext;
    /* Last selected item (row number) */
    uint8_t selection = 0;

public:
    /**
     * @brief Get the screen header (first line of the screen).
     * 
     * If this function returns 'nullptr', the screen has no header.
     * 
     * @return const char* 
     */
    virtual const char *getHeader() = 0;
    
    /**
     * @brief Get the name how the screen appears in a Menu.
     * 
     * @return const char* 
     */
    virtual const char *getMenuName() = 0;

    /**
     * @brief Set the selected row.
     * 
     * @param sel  Row number starting at 0.
     */
    void setSelection( uint8_t sel) { selection = sel; }

    /**
     * @brief Get the current selected row.
     * 
     * @return uint8_t: Current selected row.
     */
    uint8_t getSelection() { return selection; }

    /**
     * @brief Check whether the screen wants a 'Go Back' item.
     * 
     * If this method returns 'true' the user interface includes a 'Go Back' item in the screen.
     * 
     * @return bool: 'true' to include the 'Go Back' item. 
     */
    virtual bool goBackItem() { return false; }

    /**
     * @brief Screen activation callback.
     * 
     * Called before the screen gets activated and painted for the first time.
     * 
     * @param ui Pointer to TextUI.
     */
    virtual void activate(TextUI *ui)
    { /* default implementation does nothing */
    }

    /**
     * @brief Screen deactivation callback.
     * 
     * Called before the screen is deactivated and another screen is activated.
     * 
     * @param ui  Pointer to TextUI.
     */
    virtual void deactivate(TextUI *ui)
    { /* default implementation does nothing */
    }

    /**
     * @brief Process key and other events.
     *
     * If the event was handled
     * e->markProcessed() should be called to prevent the UI from also handle the same event again.
     * 
     * @param ui  Pointer to TextUI.
     * @param e   An Event.
     */
    virtual void handleEvent(TextUI *ui, Event *e)
    { /* default implementation does nothing */
    }

    /**
     * @brief Check if values of the row are editable.
     * 
     * The default implementation returns 'true'.
     * 
     * @param row uint8_t: Row number.
     * @returns bool: 'true' if this row contains editable cells.
     */
    virtual bool isRowEditable(uint8_t row) { return true; }

    /* Return true if the value of this cell is editable */

    /**
     * @brief Check if a cell is editable.
     * 
     * The default implementation returns 'false'.
     * 
     * @param row uint8_t: Row number.
     * @param col uint8_t: Column number.
     * @return bool: 'true' if this cell is editable.
     */
    virtual bool isColEditable(uint8_t row, uint8_t col) { return false; }

    /**
     * @brief Return true if rowExecute should be called when this row is activated.
     * 
     * A row gets activated when it is the currently selected row and key ENTER is pressed.
     * 
     * @param row uint8_t: row number.
     * @return bool: 
     */
    virtual bool isRowExecutable(uint8_t row) { return false; }

    /**
     * @brief Callback when a row gets activated.
     * 
     * The default implementation does nothing.
     * 
     * @param ui TextUI*: A pointer to the user interface.
     * @param row uint8_t: Method is called for this row.
     */
    virtual void rowExecute(TextUI *ui, uint8_t row)
    { /* default implementation does nothing */
    }

    /**
     * @brief Check whether the module itself is executable.
     * 
     * This will call moduleExecute() of the module if it is activated within a menu.
     * The default implementation returns 'false'.
     * 
     * @return bool: 'true' if the module is executable.
     */
    virtual bool isModuleExecutable() { return false; }

    /**
     * @brief Execute call of a screen.
     * 
     * Called when isModuleExecutable() returns true and the user presses the ENTER key in the menu.
     * The default implementation does nothing.
     * 
     * @param ui  Pointer to TextUI.
     */
    virtual void moduleExecute(TextUI *ui)
    { /* default implementation does nothing */
    }

    /**
     * @brief Get the number of screen rows.
     * 
     * @return uint8_t: Number of rows.
     */
    virtual uint8_t getRowCount() = 0;

    /**
     * @brief Get the name of a row.
     * 
     * @param row  Return name of this row.
     * 
     * @return const char*: Row name.
     */
    virtual const char *getRowName(uint8_t row) = 0;

    /**
     * @brief Get the number of screen columns.
     * 
     * @param row  Return number of columns for this row.
     * 
     * @return uint8_t: Number of columns.
     */
    virtual uint8_t getColCount(uint8_t row) = 0;

    /* Called before and after table redraw */

    /**
     * @brief Callback before screen refresh.
     * 
     * The default implementation does nothing.
     */
    virtual void startRefresh()
    { /* default implementation does nothing */
    }

    /**
     * @brief Callback after screen refresh.
     * 
     * The default implementation does nothing.
     */
    virtual void endRefresh()
    { /* default implementation does nothing */
    }

    /**
     * @brief Screen requests full repaint.
     * 
     * If this method returns true the UI clears and repaints the screen.
     * The default implemetation returns 'false'.
     * 
     * @returns bool: 'true' to indicate need for repaint.
     */
    virtual bool needsRefresh() { return false; };

    /**
     * @brief Check if a cell value has changed.
     * 
     * Return true if a value has changed without user interaction.
     * Default implementation always returns 'false'.
     * 
     * @param row  Cell row nummber starting at 0.
     * @param col  Cell column starting at 0.
     * 
     * @returns bool: 'true' if this cell has changed.
     */
    virtual bool hasChanged(uint8_t row, uint8_t col) { return false; }

    /**
     * @brief Callback to fetch current cell value.
     * 
     * The user interface used this callback to fetch the current cell value
     * for rendering and editing.
     * 
     * @param row uint8_t: Row number.
     * @param col uint8_t: Column number.
     * @param cell Cell*: An empty cell.
     */
    virtual void getValue(uint8_t row, uint8_t col, Cell *cell) = 0;

    /**
     * @brief Callback after a cell has been edited.
     * 
     * The default implementation does nothing.
     * 
     * @param row uint8_t: Row number.
     * @param col uint8_t: Column number.
     * @param cell Cell*: The modified (edited) cell.
     */
    virtual void setValue(uint8_t row, uint8_t col, Cell *cell)
    { /* default implementation does nothing */
    }
};

/**
 * @brief A simple menu class.
 * 
 */
class TextUIMenu : public TextUIScreen
{

private:
    const char *header;
    bool useGoBackItem = false;
    TextUIScreen *first = nullptr;
    TextUIScreen *last = nullptr;

public:
    /**
     * @brief Construct a new TextUIMenu.
     * 
     * The menu does not show the 'Go Back' item.
     * 
     * @param hdr const char*: The header text line.
     */
    TextUIMenu(const char *hdr);

    /**
     * @brief Construct a new TextUIMenu.
     * 
     * Optionally show the 'Go Back' item.
     * 
     * @param hdr const char*: The header text line.
     * @param goBackItem bool: If 'true' show the 'Go Back' item.
     */
    TextUIMenu(const char *hdr, bool goBackItem);

    /**
     * @brief Add a screen to the menu.
     * 
     * @param screenPtr TextUIScreen*: The screen to add to the menu.
     */
    void addScreen(TextUIScreen *screenPtr);

    /**
     * @brief Return number of screens in the menu.
     * 
     * @return uint8_t: The number of screens.
     */
    uint8_t getScreenCount();

    /**
     * @brief Get the first screen.
     * 
     * If there are no screens this function returns 'nullptr'.
     * 
     * @return TextUIScreen*: A screen or nullptr.
     */
    TextUIScreen *getFirstScreen();

    /**
     * @brief Get the next screen.
     * 
     * If 'scr' was the last screen this function returns 'nullptr'.
     * 
     * @param scr TextUIScreen*: Get screen following this screen.
     * @return TextUIScreen*: A screen or nullptr.
     */
    TextUIScreen *getNextScreen( TextUIScreen *scr);

    /**
     * @brief Get a screen by index.
     * 
     * If the index is out of range the function returns 'nullptr'.
     * 
     * @param idx uint8_t: The screen index number.
     * @return TextUIScreen*: The screen or nullptr.
     */
    TextUIScreen *getScreen(uint8_t idx);

    /* Inherited from TextUIScreen */
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

/**
 * @brief Handle screen render and cell edit.
 * 
 */
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

    Refresh_t refresh;
    Mode_t mode;

    void editCurrentCell(TextUILcd *lcd, Event *event);
    void onDemandRefresh(TextUILcd *lcd);

    bool adjustTopRow(TextUILcd *lcd);

    /**
     * @brief Refresh a single line on the screen.
     * 
     * Note: 'row' is the table row number including back item.
     *
     * @param lcd  The TextUILcd to print on.
     * @param row  Table row including back item.
     */
    void refreshLine(TextUILcd *lcd, uint8_t row);

    /**
     * @brief Refresh a single cell.
     * 
     * @param lcd  Pointer to TextUILcd to print on.
     * @param row  Row number (includes back item).
     * @param col  Column number.
     */
    void refreshCell( TextUILcd *lcd, uint8_t row, uint8_t col);

    void updateScreen(TextUILcd *lcd);
    void updateTable(TextUILcd *lcd);
    void updateRow(TextUILcd *lcd);
    void firstEditableCol(uint8_t row);
    void skipNonEditableCol(uint8_t row);
    void prev(uint8_t count);
    void next(uint8_t count);
    void clear(TextUI *ui);

public:
    /**
     * @brief Set current screen and force full refresh.
     * 
     * The first row of the screen will be the current selected row.
     *  
     * @param ui TextUI*: Pointer to the user interface.
     * @param scr TextUIScreen*: Pointer to the new active screen.
     */
    void set(TextUI *ui, TextUIScreen *scr);

    /**
     * @brief Set current screen and force full refresh.
     * 
     * This method allows to specify the current selected row.
     * 
     * @param ui TextUI*: Pointer to the user interface.
     * @param scr TextUIScreen*: Pointer to the new active screen.
     * @param currentSelected uint8_t: Current selected row.
     */
    void set(TextUI *ui, TextUIScreen *scr, uint8_t currentSelected);

    /**
     * @brief Main process method.
     * 
     * This method should only be called from TextUI class.
     * Call TextUI::handle() instead.
     * 
     * @param ui TextUI*: Pointer to the user interface.
     * @param lcd TextUILcd*: Pointer to the display.
     * @param event Event*: The event to process.
     */
    void process(TextUI *ui, TextUILcd *lcd, Event *event);

    /**
     * @brief Cancel edit mode.
     * 
     * This method should only be called from TextUI class.
     * Use TextUI::cancelEdit() instead.
     * 
     * @param toCancel TextUIScreen*: Cancel edit mode if this screen is active.
     */
    void cancelEdit(TextUIScreen *toCancel);

    /**
     * @brief Check for active edite mode.
     * 
     * @return boolean: 'true' if a cell is currently in edit mode.
     */
    boolean inEditMode();

    /**
     * @brief Force screen refresh.
     */
    void forceRefresh() { refresh = REFRESH_SCREEN; }

    /**
     * @brief Set current row and force screen refresh.
     * 
     * @param currRow new current row
     */
    void forceRefresh( uint8_t currRow) { refresh = REFRESH_SCREEN; tableRow = currRow; }
};

/**
 * @brief Main user interface class.
 * 
 */
class TextUI
{

private:
    Event event;
    TextUIHandler handler;

    TextUIScreen *homeScreen = nullptr;

    TextUIScreen *screenStack[TEXTUI_SCREENSTACK_SZ];
    uint8_t stackPtr = 0;
#define CURRENT_SCREEN (screenStack[stackPtr])

    Refresh_t refresh = REFRESH_FULL;
    uint8_t newCurrentRow = 0;
    boolean reversedNav = false;
    boolean itemPopped = false;
    TextUILcd *display = nullptr;
    TextUIInput *inputQueue = nullptr;
    TextUIInput *currentInput = nullptr;
    uiTimer_t timer_msec = 0;
    unsigned long nextTimer_msec = 0;
    unsigned long nextTick_msec = 0;

public:
    /**
     * @brief Set timer interval in milliseconds.
     * 
     * A value > 0 enables events of type EVENT_TYPE_TIMER.
     * Every 'msec' milliseconds such an event is generated.
     * A value of 0 disables timer events.
     * A value of less that 10 milliseconds is rounded to 10 milliseconds.
     * 
     * @param msec The new timer interval in milliseconds.
     */
    void setTimer(uiTimer_t msec);

    /**
     * @brief Set the Display.
     * 
     * @param lcd TextUILcd*: The display driver to use.
     */
    void setDisplay(TextUILcd *lcd);

    /**
     * @brief Get the display.
     * 
     * @return TextUILcd*: A pointer to the display.
     */
    TextUILcd *getDisplay() { return display; }

    /**
     * @brief Add an input driver.
     * 
     * An input driver is a plugin that generates events from various input devices.
     * There are input drivers for simple buttons as well as for rotary encoders.
     * Multiple input driver may be added to the user interface.
     * 
     * @param in TextUIInput*: 
     */
    void setInput(TextUIInput *in);

    /**
     * @brief Set the Home Screen.
     * 
     * This method empties the screen stack.
     * 
     * @param scr TextUIScreen*: The home screen.
     */
    void setHomeScreen(TextUIScreen *scr);

    /**
     * @brief Enable or disable reversed up/down navigation.
     * 
     * @param v If 'true' enable reversed up/down navigation.
     */
    void setReversedNav( boolean v);

    /**
     * @brief Check for reversed up/down navigation keys.
     * 
     * @return boolean: 'true' if reversed navigation is active.
     */
    boolean isReversedNav();

    /**
     * @brief Check for active edit mode.
     * 
     * @return boolean: 'true' if a cell is currently in edit mode.
     */
    boolean inEditMode();

    /**
     * @brief Cancel edit mode.
     * 
     * @param toCancel TextUIScreen*: Cancel edit mode if this screen is active.
     */
    void cancelEdit(TextUIScreen *toCancel);
    
    /**
     * @brief Get a pending event.
     * 
     * Note that this method does not block. If there is no pending event,
     * an event of type EVENT_TYPE_NONE is returned.
     * 
     * @return Event*: An event.
     */
    Event *getEvent();

    /**
     * @brief Main entry point into user interface processing.
     * 
     * This method handles all screen edit and update functionality.
     * 
     * @param ev Event*: Event to process.
     */
    void handle(Event *ev);

    /**
     * @brief Force a screen refresh.
     * 
     */
    void forceRefresh() { refresh = REFRESH_SCREEN; }

    void forceRefresh( uint8_t currRow ) { refresh = REFRESH_SCREEN_SET_ROW; newCurrentRow = currRow; }

    /**
     * @brief Activate the 'home screen'.
     * 
     * This empties the screen stack and activates the home screen.
     */
    void toHome();

    /**
     * @brief Activate a screen.
     * 
     * Emptie the screen stack and activate a different screen.
     * 
     * @param scr TextUIScreen*: Screen to activate.
     */
    void toScreen(TextUIScreen *scr);

    /**
     * @brief Replace the current screen.
     * 
     * Replace the current screen on the screen stack.
     * 
     * @param scr TextUIScreen*: Screen to activate.
     */
    void switchScreen(TextUIScreen *scr);

    /**
     * @brief Switch to a different screen.
     * 
     * Push a screen onto the screen stack and activate the new screen.
     * Note that the screen stack is of limited size. If the stack is exhausted
     * the current screen will be replaced instead.
     * 
     * @param scr TextUIScreen*: Screen to activate.
     */
    void pushScreen(TextUIScreen *scr);

    /**
     * @brief Switch to previous active screen.
     * 
     * Pops the current screen off the screen stack and activates the previous screen.
     */
    void popScreen();
};

#endif
