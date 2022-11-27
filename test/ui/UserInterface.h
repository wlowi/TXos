
#ifndef _UserInterface_h_
#define _UserInterface_h_

#include "TXos.h"
#include "SelectList.h"
#include "TableEditable.h"
#include "Menu.h"

#define SCREEN_INIT     0
#define SCREEN_HOME     1
#define SCREEN_MODEL    2
#define SCREEN_SYSTEM   3
#define SCREEN_CONFIG   4

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_UPDATE  2

class UserInterface {

    private:
        LcdWidget *lcd;

#define SCREEN_STACK_SIZE   5
        uint8_t screen[SCREEN_STACK_SIZE];
        uint8_t screenPtr;
        uint8_t refresh;
        
        TableEditable *module;
        SelectList selectList;

        void homeScreen(Event *event);
        void menuScreen(Event *event, Menu *menu);
        void configScreen(Event *event);

    public:
        void init();
        void handle();
        void switchScreen( uint8_t scr);
        void pushScreen( uint8_t scr);
        void popScreen();
};

#endif
