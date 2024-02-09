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


#ifdef ARDUINO

/* r=row name d=comm-datatype n=field name s=structure f=field c=count */
#define DICTROW( r, d, n, s, f ) \
static const DICTROW_t r PROGMEM = { d, n, offsetof( s, f), sizeof( s::f), 1 };

/* For arrays */
#define DICTROWA( r, d, n, s, f, c ) \
static const DICTROW_t r PROGMEM = { d, n, offsetof( s, f), sizeof( s::f), c };

#define DICT( d, n, ... ) \
static const DICTROW_t* const d##_rows[] PROGMEM = { __VA_ARGS__ , (const DICTROW_t*)0 }; \
static const DICT_t d##_dict PROGMEM = { n, 0 };

/* For phased config */
#define DICTP( d, n, s, ... ) \
static const DICTROW_t* const d##_rows[] PROGMEM = { __VA_ARGS__ , (const DICTROW_t*)0 }; \
static const DICT_t d##_dict PROGMEM = { n, s };

#define DICTROW_ptr( d ) (const DICTROW_t**)&d##_rows
#define DICT_ptr( d ) &d##_dict

#else

/* r=row name d=comm-datatype n=field name s=structure f=field c=count */
#define DICTROW( r, d, n, s, f ) \
static const DICTROW_t r  = { d, n, offsetof( s, f), sizeof( s::f), 1 };

/* For arrays */
#define DICTROWA( r, d, n, s, f, c ) \
static const DICTROW_t r  = { d, n, offsetof( s, f), sizeof( s::f), c };

#define DICT( d, n, ... ) \
static const DICTROW_t* const d##_rows[]  = { __VA_ARGS__ , (const DICTROW_t*)0 }; \
static const DICT_t d##_dict  = { n, 0 };

/* For phased config */
#define DICTP( d, n, s, ... ) \
static const DICTROW_t* const d##_rows[]  = { __VA_ARGS__ , (const DICTROW_t*)0 }; \
static const DICT_t d##_dict  = { n, s };

#define DICTROW_ptr( d ) (const DICTROW_t**)&d##_rows
#define DICT_ptr( d ) &d##_dict

#define pgm_read_byte( b ) (*(b))
#define pgm_read_word( b ) (*(b))
#define pgm_read_dword( b ) (*(b))
#define pgm_read_ptr_far( b ) (*(b))

#endif

class ImportExport : public Module {

    NO_CONFIG()

    private:
        uint8_t state;
        bool changed;
        Stream &inOut;
        Comm &comm;

        void exportModulePhase( const DICTROW_t* row[], uint8_t* config);

    public:
        ImportExport( Stream &stream);

        Comm& getComm() { return comm; }

        void runExport( const DICT_t *dict,  const DICTROW_t *row[], uint8_t *config, moduleSize_t configSz);

        /* From Module */
        void run( Controls &controls) final;
        void setDefaults() final;
        void exportConfig( ImportExport *exporter, uint8_t *config, moduleSize_t configSz) const {};
        
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
