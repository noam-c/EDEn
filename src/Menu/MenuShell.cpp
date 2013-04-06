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
   rocketContext(rocketContext),
   shortcutBar(gameContext, playerData, *rocketContext),
   bindings(this),
   currentState(NULL)
{
   scheduler = new Scheduler();

   rocketContext->AddReference();
   shellDocument = rocketContext->LoadDocument("data/gui/menushell.rml");

   Rocket::Core::ElementDocument* background = rocketContext->LoadDocument("data/gui/menubg.rml");

   if (background != NULL)
   {
      background->Show(Rocket::Core::ElementDocument::NONE);
      background->RemoveReference();
   }

   if(shellDocument != NULL)
   {
      shellDocument->Show();
      sidebarElement = shellDocument->GetElementById("sidebar");
      bindings.bindAction(sidebarElement, "click", &MenuShell::sidebarClicked);
   }
}

MenuShell::~MenuShell()
{
   shellDocument->RemoveReference();
   rocketContext->RemoveReference();
}

Rocket::Core::Context* MenuShell::getRocketContext() const
{
   return rocketContext;
}

Scheduler* MenuShell::getScheduler() const
{
   return scheduler;
}

Rocket::Core::ElementDocument* MenuShell::getShellDocument() const
{
   return shellDocument;
}

void MenuShell::refresh()
{
   sidebarElement->SetInnerRML("");

   std::vector<MenuShellOption> sidebarOptions = currentState->getSidebarOptions();

   bool sidebarEnabled = !sidebarOptions.empty();
   if(sidebarEnabled)
   {
      sidebarElement->SetClass("noSidebar", false);
      std::vector<MenuShellOption>::iterator iter;
      for(iter = sidebarOptions.begin(); iter != sidebarOptions.end(); ++iter)
      {
         Rocket::Core::Element* element = shellDocument->CreateElement("div");
         element->SetClass("sidebarOption", true);
         element->AppendChild(shellDocument->CreateTextNode(iter->c_str()));
         sidebarElement->AppendChild(element);
      }
   }

   sidebarElement->SetClass("noSidebar", !sidebarEnabled);
   currentState->setSidebarEnabled(sidebarEnabled);
}

void MenuShell::changeMenuState(MenuState* newState)
{
   currentState = newState;
   refresh();
}

void MenuShell::sidebarClicked(Rocket::Core::Event* event)
{
   Rocket::Core::Element* target = event->GetTargetElement();
   if(target->GetParentNode() == sidebarElement)
   {
      int childIndex = 0;

      for(;;)
      {
         target = target->GetPreviousSibling();
         if(target == NULL) break;
         ++childIndex;
      }

      currentState->sidebarClicked(childIndex);
   }
}
