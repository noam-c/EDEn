/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef DEBUG_CONSOLE_WINDOW_H
#define DEBUG_CONSOLE_WINDOW_H

#include "EdenRocketBindings.h"
#include "MessagePipe.h"

namespace Rocket
{
   namespace Controls
   {
      class ElementFormControlInput;
   };
};

/**
 * A window in which debug commands can be written and interpreted.
 *
 * @author Noam Chitayat
 */
class DebugConsoleWindow final
{
   /** The message pipe used for trigger events. */
   messaging::MessagePipe& m_messagePipe;

   /** The event binding collection for the debug window */
   EdenRocketBindings m_bindings;

   /** The Rocket context that holds this console window. */
   Rocket::Core::Context& m_context;

   /** The Rocket document representing the debug console GUI */
   Rocket::Core::ElementDocument* m_consoleDocument;

   /** The Rocket element that holds the command history. */
   Rocket::Core::Element* m_logElement;

   /** The Rocket element that holds the text box. */
   Rocket::Controls::ElementFormControlInput* m_commandElement;

   public:
      /**
       * Constructor.
       *
       * @param context The Rocket context to which this console window should attach.
       */
      DebugConsoleWindow(messaging::MessagePipe& messagePipe, Rocket::Core::Context& context);

      /**
       * Ensures that focus always lands on the textbox.
       *
       * @param event The focus event.
       */
      void onFocus(Rocket::Core::Event& event);

      /**
       * Consumes the key input used to submit commands (currently the Enter key).
       *
       * @param event The keyboard GUI event to consume.
       */
      void onKeyPress(Rocket::Core::Event& event);

      /**
       * Consumes the text change in the command prompt.
       *
       * @param event The text change event to consume.
       */
      void onTextChange(Rocket::Core::Event& event);
      
      /**
       * @return true iff the debug window is currently visible.
       */
      bool isVisible() const;

      /**
       * Reveals the debug console window.
       * (does nothing if the window is already visible)
       */
      void show();

      /**
       * Hides the debug console window from view.
       * (does nothing if the window is already hidden)
       */
      void hide();

      /**
       * Destructor.
       */
      ~DebugConsoleWindow();
};

#endif
