/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include <RmlUi/Controls.h>
#include <algorithm>
#include <iterator>

#include "DebugConsoleWindow.h"
#include "DebugCommandMessage.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ROCKET

DebugConsoleWindow::DebugConsoleWindow(messaging::MessagePipe& messagePipe, Rml::Core::Context& context) :
   m_messagePipe(messagePipe),
   m_context(context)
{
   m_consoleDocument = m_context.LoadDocument("data/gui/debugConsole.rml");
   if(m_consoleDocument != nullptr)
   {
      m_logElement = m_consoleDocument->GetElementById("commandLog");
      if(m_logElement == nullptr)
      {
         DEBUG("Warning: missing \"commandLog\" element in debug console window.");
      }

      m_commandElement = dynamic_cast<Rml::Controls::ElementFormControlInput*>(m_consoleDocument->GetElementById("commandPrompt"));
      if(m_commandElement != nullptr)
      {
         m_bindings.bindAction(m_commandElement, "keydown", [this](Rml::Core::Event& event) { onKeyPress(event); });
         m_bindings.bindAction(m_commandElement, "change", [this](Rml::Core::Event& event) { onTextChange(event); });
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
      m_consoleDocument->Close();
   }
}

void DebugConsoleWindow::onFocus(Rml::Core::Event& event)
{
   m_commandElement->Focus();
}

void DebugConsoleWindow::onTextChange(Rml::Core::Event& event)
{
   auto oldValue = m_commandElement->GetValue();
   Rml::Core::String newValue;
   std::copy_if(
      oldValue.begin(),
      oldValue.end(),
      std::back_inserter(newValue),
      [](char c) { return c != '`'; }
   );
   m_commandElement->SetValue(newValue);
}

void DebugConsoleWindow::onKeyPress(Rml::Core::Event& event)
{
   Rml::Core::Input::KeyIdentifier key = static_cast<Rml::Core::Input::KeyIdentifier>(event.GetParameter<int>("key_identifier", Rml::Core::Input::KI_UNKNOWN));

   if(key == Rml::Core::Input::KI_RETURN)
   {
      const auto text = m_commandElement->GetValue();
      if(text.length() > 0)
      {
         auto textElement = m_logElement->GetOwnerDocument()->CreateTextNode(text);
         auto entryElement = m_logElement->GetOwnerDocument()->CreateElement("div");

         entryElement->AppendChild(std::move(textElement));
         m_logElement->AppendChild(std::move(entryElement));

         m_commandElement->SetValue("");

         m_messagePipe.sendMessage(DebugCommandMessage(text.c_str()));
      }
   }
}

bool DebugConsoleWindow::isVisible() const
{
   return m_consoleDocument->IsVisible();
}

void DebugConsoleWindow::show()
{
   m_consoleDocument->Show(Rml::Core::ModalFlag::Modal);
}

void DebugConsoleWindow::hide()
{
   m_consoleDocument->Hide();
   m_commandElement->SetValue("");
}
