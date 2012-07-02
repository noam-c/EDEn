/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef EQUIP_MENU_H
#define EQUIP_MENU_H

#include "MenuState.h"
#include "guichan.hpp"
#include "ModuleSelectListener.h"
#include "ItemListModel.h"
#include <string>
#include <vector>

class EquipPane;
struct EquipSlot;
class PlayerData;
class Character;
class Item;

/**
 * The menu state that allows the player to view and change the equipment of characters in the party.
 *
 * @author Noam Chitayat
 */
class EquipMenu : public MenuState, public edwt::ModuleSelectListener
{
   /** The player data model to operate on. */
   PlayerData& playerData;
   
   /** The equipment slots of the character. */
   std::vector<EquipSlot*> equipSlots;
   
   /** The items that can be equipped in the selected slot. */
   ItemListModel equippableItems;

   /** The slot selected for re-equipping. */
   EquipSlot* selectedSlot;
   
   /** The character to display equipment info for. */
   Character* character;
   
   public:
      /**
       * Constructor.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param menuShell The shell to display the status GUI in.
       * @param playerData The player data to operate on.
       * @param character The initial character to display in the status menu.
       */
      EquipMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData, Character* character);
  
      /**
       * Fired when the player picks a new character tab.
       * Displays the character in the tab or cancels the status menu if the "Party" tab is selected.
       *
       * @param tabName The name of the newly-selected tab.
       */
      void tabChanged(const std::string& tabName);

      /**
       * Fires when an equipment slot has been selected.
       *
       * @param index The index of the equipment slot.
       */
      void moduleSelected(int index, const std::string& eventId);
   
      /**
       * Sets the character to be displayed in the menu.
       *
       * @param newCharacter The new character to display.
       */
      void setCharacter(Character* newCharacter);

      /**
       * Destructor.
       */
      ~EquipMenu();
};

#endif
