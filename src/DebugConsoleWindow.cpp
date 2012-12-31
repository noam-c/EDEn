/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "DebugConsoleWindow.h"
#include <SDL.h>
#include <string>
#include <Rocket/Controls.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_ROCKET;

DebugConsoleWindow::DebugConsoleWindow(Rocket::Core::Context& context) :
   bindings(this),
   context(context)
{
   context.AddReference();

   consoleDocument = context.LoadDocument("data/gui/debugConsole.rml");
   if(consoleDocument != NULL)
   {
      logElement = consoleDocument->GetElementById("commandLog");
      if(logElement == NULL)
      {
         DEBUG("Warning: missing \"commandLog\" element in debug console window.");
      }

      commandElement = dynamic_cast<Rocket::Controls::ElementFormControlInput*>(consoleDocument->GetElementById("commandPrompt"));
      if(commandElement != NULL)
      {
         bindings.bindAction(commandElement, "keydown", &DebugConsoleWindow::onKeyPress);
      }
      else
      {
         DEBUG("Warning: missing \"commandPrompt\" element in debug console window.");
      }
   }
}

DebugConsoleWindow::~DebugConsoleWindow()
{
   if(consoleDocument != NULL)
   {
      consoleDocument->RemoveReference();
      consoleDocument->Close();
   }

   context.RemoveReference();
}

void DebugConsoleWindow::onFocus(Rocket::Core::Event* event)
{
   commandElement->Focus();
}

void DebugConsoleWindow::onKeyPress(Rocket::Core::Event* event)
{
   Rocket::Core::Input::KeyIdentifier key = static_cast<Rocket::Core::Input::KeyIdentifier>(event->GetParameter<int>("key_identifier", Rocket::Core::Input::KI_UNKNOWN));

   if(key == Rocket::Core::Input::KI_RETURN)
   {
      Rocket::Core::String text = commandElement->GetValue();
      if(text.Length() > 0)
      {
         Rocket::Core::Element* entryElement = logElement->GetOwnerDocument()->CreateElement("div");
         Rocket::Core::ElementText* textElement = logElement->GetOwnerDocument()->CreateTextNode(text);

         entryElement->AppendChild(textElement);
         logElement->AppendChild(entryElement);

         entryElement->RemoveReference();
         textElement->RemoveReference();

         commandElement->SetValue("");

         SDL_Event event;
         event.type = SDL_USEREVENT;
         event.user.code = DEBUG_CONSOLE_EVENT;

         // Create a copy of the string that was entered into the debug console and embed it in the SDL event
         event.user.data1 = new std::string(text.CString());
         event.user.data2 = NULL;

         SDL_PushEvent(&event);
      }
   }
}

bool DebugConsoleWindow::isVisible() const
{
   return consoleDocument->IsVisible();
}

void DebugConsoleWindow::show()
{
   consoleDocument->Show(Rocket::Core::ElementDocument::MODAL);
}

void DebugConsoleWindow::hide()
{
   consoleDocument->Hide();
}