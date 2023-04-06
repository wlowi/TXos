/*
  TextUI. A simple text based UI.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "TextUI.h"

void TextUI::setTimer( uiTimer_t t_msec) {

  if( t_msec == 0) { // Disable Timer
    nextTimer_msec = timer_msec = 0;
  } else {
    timer_msec = (t_msec < 10) ? 10 : t_msec;
    nextTimer_msec = millis() + timer_msec;
  }
}

void TextUI::setDisplay( TextUILcd *lcd) {

  display = lcd;
}

void TextUI::setInput( TextUIInput *in) {

  if( inputQueue == nullptr) {
    inputQueue = in;
  } else {
    inputQueue->append( in);
  }
}

void TextUI::setHomeScreen( TextUIScreen *scr) {

  stackPtr = 0;
  CURRENT_SCREEN = homeScreen = scr;
  refresh = REFRESH_FULL;
}
    
Event *TextUI::getEvent() {

  if( timer_msec && (millis() > nextTimer_msec)) {
    
    nextTimer_msec = millis() + timer_msec;
    event.setTimerEvent();
    
  } else {

    TextUIInput *in = inputQueue;
  
    event.setNoEvent(); 
  
    while( in != nullptr) {
      if( in->pending() ) {
        in->setEvent( &event);
        break;
      }
      in = in->getNext();
    }
  }
  
  return &event;
}

void TextUI::setReversedNav( boolean v) {

  reversedNav = v;
}

boolean TextUI::isReversedNav() {

  return reversedNav;
}

boolean TextUI::inEditMode() {

  return handler.inEditMode();
}

/* Cancel edit for this table */
void TextUI::cancelEdit(TextUIScreen *toCancel) {

  handler.cancelEdit( toCancel);
}

void TextUI::handle( Event *ev) {

  if( CURRENT_SCREEN == nullptr || display == nullptr) {
    return;
  }

  if( refresh == REFRESH_FULL) {
    if( itemPopped) {
      handler.set( this, CURRENT_SCREEN, CURRENT_SCREEN->getSelection());
      itemPopped = false;
    } else {
      handler.set( this, CURRENT_SCREEN);
    }
    refresh = REFRESH_OK;
  } else if( refresh == REFRESH_SCREEN) {
    handler.forceRefresh();
    refresh = REFRESH_OK;
  }

  handler.process( this, display, ev);
}

void TextUI::toHome() {

  UILOG("toHome()\n");

  stackPtr = 0;
  CURRENT_SCREEN = homeScreen;
  refresh = REFRESH_FULL;
}

void TextUI::toScreen( TextUIScreen *scr) {

  UILOG("toScreen()\n");

  stackPtr = 0;
  CURRENT_SCREEN = scr;
  refresh = REFRESH_FULL;
}

void TextUI::switchScreen( TextUIScreen *scr) {

  UILOGV("switchScreen(): %d\n", stackPtr);

  CURRENT_SCREEN = scr;
  refresh = REFRESH_FULL;
}

void TextUI::pushScreen( TextUIScreen *scr) {
  
  if( stackPtr < TEXTUI_SCREENSTACK_SZ-1) {
    stackPtr++;
  }

  UILOGV("pushScreen(): %d\n", stackPtr);

  CURRENT_SCREEN = scr;
  refresh = REFRESH_FULL;
}

void TextUI::popScreen() {
  
  if( stackPtr > 0) {
    stackPtr--;
  }

  UILOGV("popScreen(): %d\n", stackPtr);

  refresh = REFRESH_FULL;
  itemPopped = true;
}
