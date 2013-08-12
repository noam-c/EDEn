/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "MenuShell.h"
#include "GraphicsUtil.h"
#include "MenuState.h"
#include "Scheduler.h"
#include <Rocket/Core.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

MenuShell::MenuShell(GameContext& gameContext, PlayerData& playerData, Rocket::Core::Context* rocketContext) :
   m_rocketContext(rocketContext),
   m_shortcutBar(gameContext, playerData, *rocketContext),
   m_bindings(this),
   m_currentState(NULL)
{
   m_scheduler = new Scheduler();

   m_rocketContext->AddReference();
   m_shellDocument = m_rocketContext->LoadDocument("data/gui/menushell.rml");

   Rocket::Core::ElementDocument* background = rocketContext->LoadDocument("data/gui/menubg.rml");

   if (background != NULL)
   {
      background->Show(Rocket::Core::ElementDocument::NONE);
      background->RemoveReference();
   }

   if(m_shellDocument != NULL)
   {
      m_shellDocument->Show();
      m_sidebarElement = m_shellDocument->GetElementById("sidebar");
      m_bindings.bindAction(m_sidebarElement, "click", &MenuShell::sidebarClicked);
   }
}

MenuShell::~MenuShell()
{
   m_shellDocument->RemoveReference();
   m_rocketContext->RemoveReference();
}

Rocket::Core::Context* MenuShell::getRocketContext() const
{
   return m_rocketContext;
}

Scheduler* MenuShell::getScheduler() const
{
   return m_scheduler;
}

Rocket::Core::ElementDocument* MenuShell::getShellDocument() const
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
      std::vector<MenuShellOption>::iterator iter;
      for(iter = sidebarOptions.begin(); iter != sidebarOptions.end(); ++iter)
      {
         Rocket::Core::Element* element = m_shellDocument->CreateElement("div");
         element->SetClass("sidebarOption", true);
         element->AppendChild(m_shellDocument->CreateTextNode(iter->c_str()));
         m_sidebarElement->AppendChild(element);
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

void MenuShell::sidebarClicked(Rocket::Core::Event* event)
{
   Rocket::Core::Element* target = event->GetTargetElement();
   if(target->GetParentNode() == m_sidebarElement)
   {
      int childIndex = 0;

      for(;;)
      {
         target = target->GetPreviousSibling();
         if(target == NULL) break;
         ++childIndex;
      }

      m_currentState->sidebarClicked(childIndex);
   }
}
