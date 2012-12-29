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

ItemMenu::ItemMenu(ExecutionStack& executionStack, PlayerData& playerData) :
   MenuState(executionStack, "ItemMenu"),
   bindings(this),
   playerData(playerData),
   itemViewModel(playerData)
{
   initialize();
}

ItemMenu::ItemMenu(ExecutionStack& executionStack, PlayerData& playerData, MenuShell* menuShell) :
   MenuState(executionStack, "ItemMenu", menuShell),
   bindings(this),
   playerData(playerData),
   itemViewModel(playerData)
{
   initialize();
}

void ItemMenu::initialize()
{
   paneDocument = menuShell->getContext()->LoadDocument("data/gui/itempane.rml");
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
