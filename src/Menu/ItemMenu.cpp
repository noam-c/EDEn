/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ItemMenu.h"
#include "MenuShell.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "Metadata.h"
#include "PlayerData.h"
#include "Item.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_MENU

ItemMenu::ItemMenu(GameContext& gameContext, PlayerData& playerData) :
   MenuState(gameContext, playerData, "ItemMenu"),
   m_itemViewModel(getMetadata(), playerData)
{
   initialize();
}

ItemMenu::ItemMenu(GameContext& gameContext, PlayerData& playerData, std::shared_ptr<MenuShell> menuShell) :
   MenuState(gameContext, playerData, "ItemMenu", std::move(menuShell)),
   m_itemViewModel(getMetadata(), playerData)
{
   initialize();
}

void ItemMenu::initialize()
{
   m_paneDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/itempane.rml");
   if(m_paneDocument != nullptr)
   {
      m_bindings.bindAction(m_paneDocument, "itemGrid", "click", [this](Rocket::Core::Event& event) { itemClicked(event); });
      m_bindings.bindAction(m_paneDocument, "itemGrid", "dragstart", [this](Rocket::Core::Event& event) { dragStarted(event); });
   }
}

void ItemMenu::dragStarted(Rocket::Core::Event& event)
{
   Rocket::Core::Element* dragElement = static_cast<Rocket::Core::Element*>(event.GetParameter< void* >("drag_element", nullptr));
   if (dragElement != nullptr)
   {
      Rocket::Core::Element* target = event.GetTargetElement();

      // Move up the DOM to the datagridrow item holding this element
      while(target->GetParentNode() != nullptr && target->GetTagName() != "datagridrow")
      {
         target = target->GetParentNode();
      }

      if(target != nullptr)
      {
         // If we found a row element, cast it and get its index
         Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
         if(rowElement != nullptr)
         {
            int itemIndex = rowElement->GetParentRelativeIndex();
            UsableId itemId = m_itemViewModel.getItemId(itemIndex);
            dragElement->SetAttribute("itemId", static_cast<int>(itemId));
            DEBUG("Dragging item %d.", itemId);
         }
      }
   }
}

void ItemMenu::itemClicked(Rocket::Core::Event& event)
{
   Rocket::Core::Element* target = event.GetTargetElement();

   // Move up the DOM to the datagridrow item holding this element
   while(target->GetParentNode() != nullptr && target->GetTagName() != "datagridrow")
   {
      target = target->GetParentNode();
   }

   if(target != nullptr)
   {
      // If we found a row element, cast it and get its index
      Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
      if(rowElement != nullptr)
      {
         int itemIndex = rowElement->GetParentRelativeIndex();
         useItem(itemIndex);
      }
   }
}

void ItemMenu::useItem(int itemIndex)
{
   const ItemList& itemList = m_playerData.getInventory()->getItemList();
   const UsableId usableId = itemList[itemIndex].first;
   getMetadata().useItem(usableId, getStateType());
   m_itemViewModel.refresh(itemIndex);
}
