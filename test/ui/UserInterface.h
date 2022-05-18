
#ifndef _UserInterface_h_
#define _UserInterface_h_

#include "TXos.h"
#include "DisplayBox.h"
#include "SelectList.h"
#include "TableEditable.h"

#define SCREEN_INIT     0
#define SCREEN_HOME     1
#define SCREEN_SELECT   2
#define SCREEN_CONFIG   3

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_UPDATE  2

class UserInterface {

    private:
        LcdWidget *lcd;

        uint8_t screen;
        uint8_t refresh;
        
        TableEditable *module;
        SelectList selectList;

        void homeScreen(Event *event);
        void selectScreen(Event *event);
        void configScreen(Event *event);
        
        void switchScreen( uint8_t scr);

    public:
        void init();
        void handle();
};

#endif
