
#ifndef _SwitchMonitor_h_
#define _SwitchMonitor_h_

#include "Module.h"

class SwitchMonitor : public Module {

    NO_CONFIG()

    private:
        Controls &current;
        char switchName[TEXT_SW_NAME_length+1];

    public:
        explicit SwitchMonitor( Controls &controls);
        
        void run( Controls &controls) final;
        void setDefaults() final;

        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        bool isRowEditable( uint8_t row) final { return false; }

        uint8_t getRowCount() final;
        const char *getRowName( uint8_t row) final;
        uint8_t getColCount( uint8_t row) final;
        
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
        bool hasChanged( uint8_t row, uint8_t col) final;
};

#endif