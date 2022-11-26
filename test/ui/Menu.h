
#ifndef _Menu_h_
#define _Menu_h_

#include "TXos.h"
#include "Module.h"

class Menu : public TableEditable {

    private:

        const char *header;
        Module *first = nullptr;
        Module *last = nullptr;

    public:
        explicit Menu( const char *hdr);

        void setDefaults();

        void addModule( Module *modulePtr);

        uint8_t getModuleCount();
        Module *getFirstModule();
        Module *getModule( uint8_t idx);
        Module *getModuleByType( moduleType_t type);

        /* From Interface TableEditable */
        bool isRowEditable( uint8_t row) final { return false; }
        bool isRowExecutable( uint8_t row) final;
        void rowExecute( uint8_t row ) final;

        const char *getName() final;
        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

};

#endif
