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

#ifndef _ImportExport_h_
#define _ImportExport_h_

#include "Module.h"

/* A dictionary describes fields to export and import via ImportExport class.
 *
 * To save memory the dictionary is stored in PROGMEM.
 * This helper macros make it easier to defined the dictionary data structures.
 *
 */
typedef struct DICTROW_t {
    uint8_t dataType;
    nameType_t rowName;
    size_t offset;
    size_t size;
    uint16_t count;
} DICTROW_t;

typedef struct DICT_t {
    nameType_t name;
    nameType_t subName;
} DICT_t;


/* r=row name n=field name s=structure f=field c=count */
#define DICTROW( r, n, s, f, c ) \
static const DICTROW_t r PROGMEM = { 1, n, offsetof( s, f), sizeof( s::f), c };

#define DICT( d, n, s, ... ) \
static const DICT_t d##_dict PROGMEM = { n, s }; \
static const DICTROW_t* const d##_rows[] PROGMEM = { __VA_ARGS__ };

#define DICTROW_P( d ) &d##_rows
#define DICT_P( d ) &d##_dict



class ImportExport : public Module {

    NO_CONFIG()

    private:
        uint8_t state;
        bool changed;
        Stream &inOut;
        Comm &comm;

    public:
        ImportExport( Stream &stream);

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        void exportConfig( Comm *exporter, uint8_t *config, moduleSize_t configSz) const {};
        
        void moduleEnter();
        void moduleExit();

        /* From TableEditable */
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;

        bool isRowEditable( uint8_t row) final { return false; }
        bool isRowExecutable( uint8_t row) final { return false; };
        bool hasChanged( uint8_t row, uint8_t col) final;
};

#endif
