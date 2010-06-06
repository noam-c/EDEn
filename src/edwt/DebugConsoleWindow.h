#ifndef __DEBUG_CONSOLE_WINDOW_H_
#define __DEBUG_CONSOLE_WINDOW_H_

#include "guichan.hpp"

const int DEBUG_CONSOLE_EVENT = 10;

namespace edwt
{
   class TextBox;
   class TextField;

   class DebugConsoleWindow : public gcn::Window, public gcn::KeyListener
   {
      gcn::ScrollArea* consoleLogScroll;
      TextBox* consoleLog;
      TextField* consoleInput;

      public:
         DebugConsoleWindow(gcn::Container* container, int width, int height);
         void requestFocus();
         void keyPressed(gcn::KeyEvent& keyEvent);
         ~DebugConsoleWindow();
   };
};

#endif