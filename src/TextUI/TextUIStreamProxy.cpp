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

#include "TextUIStreamProxy.h"


// #define SERIAL_DEBUG

TextUIStreamProxy::TextUIStreamProxy(Stream& s) : stream(s) {

    stream.setTimeout(100);
    sync();
}

/* TextUIInput */
bool TextUIStreamProxy::pending() {

    checkInput(true /* noWait */);
    return eventPending;
}

void TextUIStreamProxy::setEvent(Event* e) {

    e->setNoEvent();

    if (eventPending) {
        e->setKeyEvent(key, count);
        eventPending = false;
    }
}

/* TextUILcd */
void TextUIStreamProxy::clear() {

    simpleCommand(CMD_CLEAR);
}

void TextUIStreamProxy::clearEOL() {

    simpleCommand(CMD_CLEAREOL);
}

bool TextUIStreamProxy::colorSupport() {

    queryCommand(COMMAND_GET_COLORSUPPORT);
    return receiveData();
}

void TextUIStreamProxy::setBg(uint8_t r, uint8_t g, uint8_t b) {

    byteCommand3(COMMAND_SET_BG, r, g, b);
}

void TextUIStreamProxy::setFg(uint8_t r, uint8_t g, uint8_t b) {

    byteCommand3(COMMAND_SET_FG, r, g, b);
}

void TextUIStreamProxy::normalColors() {

    simpleCommand(CMD_SET_NORMAL);
}

void TextUIStreamProxy::selectedColors() {

    simpleCommand(CMD_SET_SELECTED);
}

void TextUIStreamProxy::editColors() {

    simpleCommand(CMD_SET_EDIT);
}

void TextUIStreamProxy::setInvert(bool inv) {

    byteCommand1(COMMAND_SET_INVERT, (uint8_t)inv);
}

/* TEXTUI_FONT_SMALL .. TEXTUI_FONT_LARGE */
void TextUIStreamProxy::setFontSize(FontSize_t sz) {

    byteCommand1(COMMAND_SET_FONTSIZE, (uint8_t)sz);
}

uint8_t TextUIStreamProxy::getRows() {

    queryCommand(COMMAND_GET_ROWS);
    return receiveData();
}

uint8_t TextUIStreamProxy::getColumns() {

    queryCommand(COMMAND_GET_COLUMNS);
    return receiveData();
}

/* row and column in characters */
void TextUIStreamProxy::setCursor(uint8_t r, uint8_t c) {

    toCommandMode();
    send(CMD_SET_CURSOR);
    sendByte(r);
    sendByte(c);
    currentMode = PRINT_MODE;
}

void TextUIStreamProxy::setRow(uint8_t r) {

    toCommandMode();
    send(CMD_SET_ROW);
    sendByte(r);
    currentMode = PRINT_MODE;
}

void TextUIStreamProxy::setColumn(uint8_t c) {

    toCommandMode();
    send(CMD_SET_COLUMN);
    sendByte(c);
    currentMode = PRINT_MODE;
}

void TextUIStreamProxy::printChar(char ch) {

    toPrintMode();
    if (IS_PRINTABLE(ch)) {
        send(ch);
    }
}

/** PRIVATE **/

void TextUIStreamProxy::toCommandMode() {

    if (currentMode != COMMAND_MODE) {
        send(CMD_ATTN);
        currentMode = COMMAND_MODE;
    }
}

void TextUIStreamProxy::toPrintMode() {

    if (currentMode != PRINT_MODE) {
        send(CMD_PRINT);
        currentMode = PRINT_MODE;
    }
}

void TextUIStreamProxy::simpleCommand(char cmd) {

    toCommandMode();
    send(cmd);
    currentMode = PRINT_MODE;
}

void TextUIStreamProxy::byteCommand1(commandType_t cmd, uint8_t u) {

    toCommandMode();
    send(CMD_COMMAND);
    send(COMMAND_CH1(cmd));
    send(COMMAND_CH2(cmd));
    sendByte(u);
    send(CMD_END);
}

void TextUIStreamProxy::byteCommand2(commandType_t cmd, uint8_t u, uint8_t v) {

    toCommandMode();
    send(CMD_COMMAND);
    send(COMMAND_CH1(cmd));
    send(COMMAND_CH2(cmd));
    sendByte(u);
    sendByte(v);
    send(CMD_END);
}

void TextUIStreamProxy::byteCommand3(commandType_t cmd, uint8_t u, uint8_t v, uint8_t w) {

    toCommandMode();
    send(CMD_COMMAND);
    send(COMMAND_CH1(cmd));
    send(COMMAND_CH2(cmd));
    sendByte(u);
    sendByte(v);
    sendByte(w);
    send(CMD_END);
}

void TextUIStreamProxy::queryCommand(commandType_t cmd) {

    toCommandMode();
    send(CMD_QUERY);
    send(COMMAND_CH1(cmd));
    send(COMMAND_CH2(cmd));
    send(CMD_END);
}

void TextUIStreamProxy::send(char ch) {

    stream.write(ch);

#ifdef SERIAL_DEBUG
    Serial.write(ch);
#endif
}

bool TextUIStreamProxy::readChar(char* b) {

    /* Read with timeout */
    return (stream.readBytes(b, 1) == 1);
}

void TextUIStreamProxy::sendByte(uint8_t b) {

    if( b < 64) {
        stream.write(b + SINGLECHAR_OFFSET);
    } else {
        stream.write(((b >> 4) & 0x0f) + TWOCHAR_OFFSET);
        stream.write((b & 0x0f) + TWOCHAR_OFFSET);
    }

#ifdef SERIAL_DEBUG
    Serial.print(ch);
    Serial.write(';');
#endif
}

bool TextUIStreamProxy::readByte(uint8_t* b) {

    uint8_t buffer[2];

    if( stream.readBytes( &buffer[0], 1) == 1) {
        if( buffer[0] >= SINGLECHAR_OFFSET) {
            *b = (buffer[0] - SINGLECHAR_OFFSET);
            return true;
        } else if( (buffer[0] >= TWOCHAR_OFFSET) && (stream.readBytes( &buffer[1], 1) == 1) ) {
            *b = ((buffer[0] - TWOCHAR_OFFSET) << 4) | (buffer[1] - TWOCHAR_OFFSET);
            return true;
        }
    }
    
    return false;
}

void TextUIStreamProxy::sync() {

#ifdef SERIAL_DEBUG
    Serial.println("SYNC");
#endif

    for (uint8_t i = 0; i < 4; i++) {
        stream.write(CMD_ATTN);
    }

    while(stream.available()) {
        stream.read();
    }

    currentMode = COMMAND_MODE;
}

void TextUIStreamProxy::checkInput(bool noWait) {

    char ch;
    uint8_t buffer;

    if (noWait && !stream.available()) {
        return;
    }

    if ( readChar( &ch)) {

        switch (ch) {
        case CMD_QUERY_RESULT:
            /* Read with timeout */
            if ( readByte( &buffer)) {
                data = buffer;
                dataPending = true;
            }
            else {
                sync();
            }
            break;

        case CMD_KBD:
            if ( readByte( &buffer)) {
                key = buffer;
                if ( readByte( &buffer)) {
                    count = buffer;
                    eventPending = true;
                }
            }
            else {
                sync();
            }
            break;
        }
    }
}

uint8_t TextUIStreamProxy::receiveData() {

    uint8_t retry = 20;

    while (!dataPending && (retry-- > 0)) {
        checkInput(false /* wait */);
    }

    if (dataPending) {
        dataPending = false;
#ifdef SERIAL_DEBUG
        Serial.write('=');
        Serial.println(data);
#endif
        return data;
    }
    else {
#ifdef SERIAL_DEBUG
        Serial.write('=');
        Serial.println(0);
#endif
        return 0; /** @TODO fix */
    }
}
