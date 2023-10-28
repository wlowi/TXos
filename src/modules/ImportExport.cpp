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

#include "ImportExport.h"
#include "ModuleManager.h"

extern ModuleManager moduleManager;

const uint8_t STATE_INACTIVE = 0;
const uint8_t STATE_CONNECTING = 1;

ImportExport::ImportExport() : Module( MODULE_IMPORTEXPORT_TYPE, TEXT_MODULE_IMPORTEXPORT) {

    setDefaults();
}

/* From Module */

/* Initial state is STATE_CONNECTING.
 * In this state we are waiting for a connnect request.
 * Once the connect request is received we send an info packet.
 * The info packet contains the firmware release and the number of supported models.
*/

void ImportExport::run( Controls &controls) {

    switch( state) {
        case STATE_CONNECTING:
            break;

        case STATE_INACTIVE:
        default:
            break;
    }
    /* noop */
}

void ImportExport::setDefaults() {

    state = STATE_INACTIVE;
    changed = true;
}

void ImportExport::moduleEnter() {

    state = STATE_CONNECTING;
    changed = true;
}
        
void ImportExport::moduleExit() {

    setDefaults();
}

/* From TextUIScreen */

bool ImportExport::hasChanged( uint8_t row, uint8_t col) {

    bool ret = false;
    
    if( row == 2) {
        ret = changed;
        changed = false;
    }

    return ret;
}

uint8_t ImportExport::getRowCount() {

    return 3;
}

const char *ImportExport::getRowName( uint8_t row) {

    if( row == 1) {
        return TEXT_STATUS;
    }

    return "";
}

uint8_t ImportExport::getColCount( uint8_t row) {

    if( row == 2) {
        return 1;
    }

    return 0;
}

void ImportExport::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 2 ) {
        switch( state) {

            case STATE_INACTIVE:
                cell->setLabel( 0, TEXT_OFF, strlen(TEXT_OFF));
                break;

            case STATE_CONNECTING:
                cell->setLabel( 0, TEXT_CONNECTING, strlen(TEXT_CONNECTING));
                break;
        }
    }
}
