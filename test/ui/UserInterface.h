
#ifndef _UserInterface_h_
#define _UserInterface_h_

#include "TXos.h"
#include "DisplayBox.h"

#define SCREEN_INIT     0
#define SCREEN_HOME     1
#define SCREEN_SELECT   2
#define SCREEN_CONFIG   3

#define REFRESH_OK      0
#define REFRESH_FULL    1
#define REFRESH_UPDATE  2

class SelectList {

    private:
    
    public:
        void clear();
        void add();
};

class UserInterface {

    private:
        LcdWidget *lcd;

        uint8_t screen;
        uint8_t refresh;
        
        void homeScreen();
        void selectScreen();
        void configScreen();
        
        void switchScreen( uint8_t scr);

    public:
        void init();
        void handle();
};

#endif
