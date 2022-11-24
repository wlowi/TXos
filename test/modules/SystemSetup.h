
#ifndef _SystemSetup_h_
#define _SystemSetup_h_

#include "TXos.h"
#include "Module.h"

class SystemSetup : public Module {

    public:
        explicit SystemSetup();
        
        /* From MOdule */
        void run( Controls &controls) final;
        void setDefaults() final;

        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        bool isEditable( uint8_t row) final { return false; }
        bool isModuleExecutable() final { return true; }

        void moduleExecute() final;

        uint8_t getItemCount() final;
        const char *getItemName( uint8_t row) final;
        uint8_t getValueCount() final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif