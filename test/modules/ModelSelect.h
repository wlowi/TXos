
#ifndef _ModelSelect_h_
#define _ModelSelect_h_

#include "TXos.h"
#include "ModuleManager.h"
#include "Model.h"

class ModelSelect : public Module {

    private:
        char modelNo[3];
        Model model;
        
    public:
        ModelSelect();

        /* From Module */
        void run( channelSet_t &channels);
        void setDefaults();
        moduleSize_t getConfigSize();
        uint8_t *getConfig();

        /* From TableEditable */
        bool isEditable() { return false; }; // override
        bool isExecutable() { return true; }; // override
        void execute( uint8_t row );

        uint8_t getItemCount();
        const char *getItemName( uint8_t row);
        uint8_t getValueCount();
        void getValue( uint8_t row, uint8_t col, Cell *cell);
        void setValue( uint8_t row, uint8_t col, Cell *cell);
};

#endif
