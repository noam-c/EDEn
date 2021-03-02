/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "MenuShell.h"
#include "GraphicsUtil.h"
#include "MenuState.h"
#include "ShortcutBar.h"
#include <RmlUi/Core.h>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_MENU

MenuShell::MenuShell(Rml::Core::Context* rocketContext) :
   m_rocketContext(rocketContext)
{
   m_shellDocument = m_rocketContext->LoadDocument("data/gui/menushell.rml");

   auto background = m_rocketContext->LoadDocument("data/gui/menubg.rml");
   if (background != nullptr)
   {
      background->Show(Rml::Core::ModalFlag::None);
   }

   if(m_shellDocument != nullptr)
   {
      m_shellDocument->Show();
      m_sidebarElement = m_shellDocument->GetElementById("sidebar");
      m_bindings.bindAction(m_sidebarElement, "click", [this](Rml::Core::Event& event) { sidebarClicked(event); });
   }
}

MenuShell::~MenuShell() = default;

void MenuShell::initializeShortcutBar(PlayerData& playerData, Metadata& metadata, GameStateType stateType)
{
   if(!m_shortcutBar)
   {
      m_shortcutBar = std::unique_ptr<const ShortcutBar>(new ShortcutBar(playerData, metadata, stateType, *m_rocketContext));
   }
}

Rml::Core::Context* MenuShell::getRocketContext() const
{
   return m_rocketContext;
}

Scheduler* MenuShell::getScheduler()
{
   return &m_scheduler;
}

Rml::Core::ElementDocument* MenuShell::getShellDocument() const
{
   return m_shellDocument;
}

void MenuShell::refresh()
{
   m_sidebarElement->SetInnerRML("");

   std::vector<MenuShellOption> sidebarOptions = m_currentState->getSidebarOptions();

   bool sidebarEnabled = !sidebarOptions.empty();
   if(sidebarEnabled)
   {
      m_sidebarElement->SetClass("noSidebar", false);
      for(const auto& sidebarOption : sidebarOptions)
      {
         auto element = m_shellDocument->CreateElement("div");
         element->SetClass("sidebarOption", true);
         element->AppendChild(m_shellDocument->CreateTextNode(sidebarOption.c_str()));
         m_sidebarElement->AppendChild(std::move(element));
      }
   }

   m_sidebarElement->SetClass("noSidebar", !sidebarEnabled);
   m_currentState->setSidebarEnabled(sidebarEnabled);
}

void MenuShell::changeMenuState(MenuState* newState)
{
   m_currentState = newState;
   refresh();
}

void MenuShell::sidebarClicked(Rml::Core::Event& event)
{
   auto target = event.GetTargetElement();
   if(target->GetParentNode() == m_sidebarElement)
   {
      int childIndex = 0;

      for(;;)
      {
         target = target->GetPreviousSibling();
         if(target == nullptr) break;
         ++childIndex;
      }

      m_currentState->sidebarClicked(childIndex);
   }
}
