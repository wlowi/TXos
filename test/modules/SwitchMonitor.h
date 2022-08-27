
#ifndef _SwitchMonitor_h_
#define _SwitchMonitor_h_

#include "TXos.h"
#include "Module.h"
#include "Controls.h"

class SwitchMonitor : public Module {

    private:
        Controls &current;

    public:
        explicit SwitchMonitor( Controls &controls);
        
        void run( Controls &controls) final;
        void setDefaults() final;

        moduleSize_t getConfigSize() final;
        uint8_t *getConfig() final;

        /* From TableEditable */
        bool isEditable() final { return false; }
        bool hasChanged() final;

        uint8_t getItemCount() final;
        const char *getItemName( uint8_t row) final;
        uint8_t getValueCount() final;
        void getValue( uint8_t row, uint8_t col, Cell *cell) final;
        void setValue( uint8_t row, uint8_t col, Cell *cell) final;
};

#endif