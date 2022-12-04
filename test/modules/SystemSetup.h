
#ifndef _SystemSetup_h_
#define _SystemSetup_h_

#include "Module.h"

class SystemSetup : public Module {

    NO_CONFIG()
    
    public:
        explicit SystemSetup();
        
        /* From MOdule */
        void run( Controls &controls) final;
        void setDefaults() final;

        /* From TableEditable */
        bool isRowEditable( uint8_t row) final { return false; }
        bool isModuleExecutable() final { return true; }

        void moduleExecute() final;

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif