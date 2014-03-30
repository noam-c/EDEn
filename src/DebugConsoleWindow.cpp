/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "DebugConsoleWindow.h"
#include <SDL.h>
#include <string>
#include <Rocket/Controls.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_ROCKET;

DebugConsoleWindow::DebugConsoleWindow(Rocket::Core::Context& context) :
   m_bindings(this),
   m_context(context)
{
   m_context.AddReference();

   m_consoleDocument = context.LoadDocument("data/gui/debugConsole.rml");
   if(m_consoleDocument != nullptr)
   {
      m_logElement = m_consoleDocument->GetElementById("commandLog");
      if(m_logElement == nullptr)
      {
         DEBUG("Warning: missing \"commandLog\" element in debug console window.");
      }

      m_commandElement = dynamic_cast<Rocket::Controls::ElementFormControlInput*>(m_consoleDocument->GetElementById("commandPrompt"));
      if(m_commandElement != nullptr)
      {
         m_bindings.bindAction(m_commandElement, "keydown", &DebugConsoleWindow::onKeyPress);
      }
      else
      {
         DEBUG("Warning: missing \"commandPrompt\" element in debug console window.");
      }
   }
}

DebugConsoleWindow::~DebugConsoleWindow()
{
   if(m_consoleDocument != nullptr)
   {
      m_consoleDocument->RemoveReference();
      m_consoleDocument->Close();
   }

   m_context.RemoveReference();
}

void DebugConsoleWindow::onFocus(Rocket::Core::Event* event)
{
   m_commandElement->Focus();
}

void DebugConsoleWindow::onKeyPress(Rocket::Core::Event* event)
{
   Rocket::Core::Input::KeyIdentifier key = static_cast<Rocket::Core::Input::KeyIdentifier>(event->GetParameter<int>("key_identifier", Rocket::Core::Input::KI_UNKNOWN));

   if(key == Rocket::Core::Input::KI_RETURN)
   {
      Rocket::Core::String text = m_commandElement->GetValue();
      if(text.Length() > 0)
      {
         Rocket::Core::Element* entryElement = m_logElement->GetOwnerDocument()->CreateElement("div");
         Rocket::Core::ElementText* textElement = m_logElement->GetOwnerDocument()->CreateTextNode(text);

         entryElement->AppendChild(textElement);
         m_logElement->AppendChild(entryElement);

         entryElement->RemoveReference();
         textElement->RemoveReference();

         m_commandElement->SetValue("");

         SDL_Event event;
         event.type = SDL_USEREVENT;
         event.user.code = DEBUG_CONSOLE_EVENT;

         // Create a copy of the string that was entered into the debug console and embed it in the SDL event
         event.user.data1 = new std::string(text.CString());
         event.user.data2 = nullptr;

         SDL_PushEvent(&event);
      }
   }
}

bool DebugConsoleWindow::isVisible() const
{
   return m_consoleDocument->IsVisible();
}

void DebugConsoleWindow::show()
{
   m_consoleDocument->Show(Rocket::Core::ElementDocument::MODAL);
}

void DebugConsoleWindow::hide()
{
   m_consoleDocument->Hide();
}
