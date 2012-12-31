/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ItemMenu.h"
#include "MenuShell.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "PlayerData.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

ItemMenu::ItemMenu(GameContext& gameContext, PlayerData& playerData) :
   MenuState(gameContext, "ItemMenu"),
   bindings(this),
   playerData(playerData),
   itemViewModel(gameContext, playerData)
{
   initialize();
}

ItemMenu::ItemMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell) :
   MenuState(gameContext, "ItemMenu", menuShell),
   bindings(this),
   playerData(playerData),
   itemViewModel(gameContext, playerData)
{
   initialize();
}

void ItemMenu::initialize()
{
   paneDocument = menuShell->getRocketContext()->LoadDocument("data/gui/itempane.rml");
   if(paneDocument != NULL)
   {
      bindings.bindAction(paneDocument, "itemGrid", "click", &ItemMenu::itemClicked);
   }
}

ItemMenu::~ItemMenu()
{
   if(paneDocument != NULL)
   {
      paneDocument->Close();
      paneDocument->RemoveReference();
   }
}

void ItemMenu::activate()
{
   MenuState::activate();
   if(paneDocument != NULL)
   {
      paneDocument->Show();
   }
}

void ItemMenu::deactivate()
{
   if(paneDocument != NULL)
   {
      paneDocument->Hide();
   }
}

void ItemMenu::itemClicked(Rocket::Core::Event* event)
{
   Rocket::Core::Element* target = event->GetTargetElement();

   // Move up the DOM to the datagridrow item holding this element
   while(target->GetParentNode() != NULL && target->GetTagName() != "datagridrow")
   {
      target = target->GetParentNode();
   }

   if(target != NULL)
   {
      // If we found a row element, cast it and get its index
      Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
      if(rowElement != NULL)
      {
         int itemIndex = rowElement->GetParentRelativeIndex();
         itemViewModel.useItem(itemIndex);
      }
   }
}
