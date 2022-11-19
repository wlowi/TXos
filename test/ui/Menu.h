
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
        Menu( const char *hdr);

        void addModule( Module *modulePtr);

        uint8_t getModuleCount();
        Module *getFirstModule();
        Module *getModule( uint8_t idx);
        Module *getModuleByType( moduleType_t type);

        /* From Interface TableEditable */
        const char *getName() final;
        uint8_t getItemCount() final;
        const char *getItemName( uint8_t row) final;
        uint8_t getValueCount() final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;

};

#endif
