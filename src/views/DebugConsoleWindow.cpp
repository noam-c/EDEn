/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include <Rocket/Controls.h>

#include "DebugConsoleWindow.h"
#include "DebugCommandMessage.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_ROCKET;

DebugConsoleWindow::DebugConsoleWindow(messaging::MessagePipe& messagePipe, Rocket::Core::Context& context) :
   m_messagePipe(messagePipe),
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
         m_bindings.bindAction(m_commandElement, "keydown", [this](Rocket::Core::Event& event) { onKeyPress(event); });
         m_bindings.bindAction(m_commandElement, "change", [this](Rocket::Core::Event& event) { onTextChange(event); });
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

void DebugConsoleWindow::onFocus(Rocket::Core::Event& event)
{
   m_commandElement->Focus();
}

void DebugConsoleWindow::onTextChange(Rocket::Core::Event& event)
{
   Rocket::Core::String value = m_commandElement->GetValue();
   m_commandElement->SetValue(value.Replace("`", ""));
}

void DebugConsoleWindow::onKeyPress(Rocket::Core::Event& event)
{
   Rocket::Core::Input::KeyIdentifier key = static_cast<Rocket::Core::Input::KeyIdentifier>(event.GetParameter<int>("key_identifier", Rocket::Core::Input::KI_UNKNOWN));

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

         m_messagePipe.sendMessage(DebugCommandMessage(text.CString()));
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
   m_commandElement->SetValue("");
}
