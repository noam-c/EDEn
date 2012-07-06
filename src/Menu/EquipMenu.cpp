/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "EquipMenu.h"
#include "EquipPane.h"
#include "PlayerData.h"
#include "CharacterRoster.h"
#include "Character.h"
#include "MenuShell.h"
#include "Tab.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

EquipMenu::EquipMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData, Character* character) : MenuState(executionStack, menuShell), playerData(playerData), character(character)
{
   // Initialize the equipment pane using the character slots.
   std::vector<EquipSlot*> characterSlots = character->getEquipment().getSlots();
   equipSlots.assign(characterSlots.begin(), characterSlots.end());
   EquipPane* equipPane = new EquipPane(equipSlots, equippableItems, menuShell.getDimension());

   // The equipment menu will listen for slot selection and item selection
   equipPane->setModuleSelectListener(this);
   setMenuPane(equipPane);
}

void EquipMenu::tabChanged(int index)
{
   if(index <= 0)
   {
      finished = true;
   }
   else
   {
      setCharacter(playerData.getRoster()->getParty()[index - 1]);
   }
}

void EquipMenu::moduleSelected(int index, const std::string& eventId)
{
   if(eventId == EquipPane::ItemListEventId)
   {
      // An item was selected from the list of available equipment.
      DEBUG("Item %d selected.", index);
      
      if(playerData.changeEquipment(character, selectedSlot, equippableItems.getItemAt(index)))
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
      ItemList acceptedItems = playerData.getInventory()->getItemsByTypes(selectedSlot->acceptedTypes);
      equippableItems.setItems(acceptedItems);
      
      ((EquipPane*)menuPane)->invalidate();
   }
}

void EquipMenu::setCharacter(Character* newCharacter)
{
   character = newCharacter;
   std::vector<EquipSlot*> characterSlots = character->getEquipment().getSlots();
   equipSlots.assign(characterSlots.begin(), characterSlots.end());

   ((EquipPane*)menuPane)->invalidate();
}

EquipMenu::~EquipMenu()
{
}
