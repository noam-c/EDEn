/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "EquipMenu.h"
#include "EquipPane.h"
#include "PlayerData.h"
#include "Character.h"
#include "MenuShell.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

EquipMenu::EquipMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData, const std::string& characterName) : MenuState(executionStack, menuShell), playerData(playerData), characterName(characterName)
{
   // Initialize the equipment pane using the character slots.
   std::vector<EquipSlot*> characterSlots = playerData.getPartyCharacter(characterName)->getEquipment().getSlots();
   equipSlots.assign(characterSlots.begin(), characterSlots.end());
   EquipPane* equipPane = new EquipPane(equipSlots, equippableItems, menuShell.getDimension());

   // The equipment menu will listen for slot selection and item selection
   equipPane->setModuleSelectListener(this);
   setMenuPane(equipPane);
}

void EquipMenu::tabChanged(const std::string& tabName)
{
   if(tabName == "Party")
   {
      finished = true;
   }
   else
   {
      setCharacter(tabName);
   }
}

void EquipMenu::moduleSelected(int index, const std::string& eventId)
{
   if(eventId == EquipPane::ItemListEventId)
   {
      // An item was selected from the list of available equipment.
      DEBUG("Item %d selected.", index);
      
      if(playerData.changeEquipment(playerData.getPartyCharacter(characterName), selectedSlot, equippableItems.getItemAt(index)))
      {
         selectedSlot = NULL;
         equippableItems.clear();
      }

      ((EquipPane*)menuPane)->invalidate();   
   }
   else if(eventId == EquipPane::SlotModuleEventId)
   {
      // An equipment slot was selected to change.
      DEBUG("Equipment slot %d selected.", index);
      selectedSlot = equipSlots[index];
      ItemList acceptedItems = playerData.getItemsByTypes(selectedSlot->acceptedTypes);
      equippableItems.setItems(acceptedItems);
      
      ((EquipPane*)menuPane)->invalidate();
   }
}

void EquipMenu::setCharacter(const std::string& charName)
{
   characterName = charName;
   std::vector<EquipSlot*> characterSlots = playerData.getPartyCharacter(characterName)->getEquipment().getSlots();
   equipSlots.assign(characterSlots.begin(), characterSlots.end());

   ((EquipPane*)menuPane)->invalidate();
}

EquipMenu::~EquipMenu()
{
}
