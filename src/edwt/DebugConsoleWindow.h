/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef DEBUG_CONSOLE_WINDOW_H
#define DEBUG_CONSOLE_WINDOW_H

#include "guichan.hpp"

const int DEBUG_CONSOLE_EVENT = 10;

namespace edwt
{
   class TextBox;
   class TextField;

   /**
    * A window in which debug commands can be written and interpreted.
    *
    * @author Noam Chitayat
    */
   class DebugConsoleWindow : public gcn::Window, public gcn::KeyListener
   {
      /** The scrolling pane containing the console log. */
      gcn::ScrollArea* consoleLogScroll;
      
      /** The text log of commands and output. */
      TextBox* consoleLog;
      
      /** The input field where a user can enter debug commands. */
      TextField* consoleInput;

      public:
         /**
          * Constructor.
          *
          * @param container The top-level container to which this console window should attach.
          * @param width The preferred width for this window.
          * @param height The preferred height for this window.
          */
         DebugConsoleWindow(gcn::Container* container, int width, int height);
      
         /**
          * Overrides the default focus handling to force focus on the console input field.
          */
         void requestFocus();
      
         /**
          * Consumes the key input used to submit commands (currently the Enter key).
          *
          * @param keyEvent The keyboard GUI event to consume.
          */
         void keyPressed(gcn::KeyEvent& keyEvent);
      
         /**
          * Destructor.
          */
         ~DebugConsoleWindow();
   };
};

#endif
