#include "DebugConsoleWindow.h"
#include <SDL.h>
#include "TextBox.h"
#include "TextField.h"
#include <string>

#include "DebugUtils.h"
const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   DebugConsoleWindow::DebugConsoleWindow(gcn::Container* container, int width, int height)
   {
      setSize(width, height);

      setVisible(false);
      setOpaque(false);
      setCaption("Debug Console");

      consoleInput = new TextField();
      consoleInput->setWidth(width);
      consoleInput->addKeyListener(this);

      int consoleLogHeight = height - (consoleInput->getHeight() + getTitleBarHeight());

      consoleLog = new TextBox();
      consoleLog->setOpaque(false);
      consoleLog->setEditable(false);
      consoleLog->setFocusable(false);

      consoleLogScroll = new gcn::ScrollArea(consoleLog);
      consoleLogScroll->setSize(width, consoleLogHeight);
      consoleLogScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

      int consoleLogWidth = width - (consoleLogScroll->getScrollbarWidth() + getPadding());
      consoleLog->setSize(consoleLogWidth, consoleLogHeight);

      add(consoleLogScroll, 0, 0);
      add(consoleInput, 0, consoleLogHeight);
      moveToTop(consoleInput);

      container->add(this);
   }

   void DebugConsoleWindow::requestFocus()
   {
      consoleInput->requestFocus();
   }

   void DebugConsoleWindow::keyPressed(gcn::KeyEvent& keyEvent)
   {
      if(keyEvent.getKey() == gcn::Key::ENTER)
      {
         std::string text = consoleInput->getText();
         if(text.length() > 0)
         {
            consoleInput->setText("");
            consoleLog->addRow(text);

            SDL_Event event;
            event.type = SDL_USEREVENT;
            event.user.code = DEBUG_CONSOLE_EVENT;

            // Create a copy of the string that was entered into the debug console and embed it in the SDL event
            event.user.data1 = new std::string(text);

            event.user.data2 = 0;
            SDL_PushEvent(&event);
         }
      }
   }

   DebugConsoleWindow::~DebugConsoleWindow()
   {
      delete consoleLog;
      delete consoleLogScroll;
      delete consoleInput;
   }
};