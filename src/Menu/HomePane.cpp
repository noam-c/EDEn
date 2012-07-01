/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "HomePane.h"
#include "PlayerData.h"
#include "CharacterModule.h"
#include "ModuleSelectListener.h"

HomePane::HomePane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
   characterModules = new CharacterModule[PlayerData::PARTY_SIZE];

   const int moduleHeight = rect.height / PlayerData::PARTY_SIZE;
   for(int i = 0; i < PlayerData::PARTY_SIZE; ++i)
   {
      characterModules[i].setHeight(moduleHeight);
      characterModules[i].setWidth(rect.width);
      characterModules[i].addActionListener(this);

      add(characterModules + i, 0, i * moduleHeight);
   }

   refresh();
}

void HomePane::setModuleSelectListener(edwt::ModuleSelectListener* listener)
{
   moduleSelectListener = listener;
}

void HomePane::refresh()
{
   const std::vector<Character*>& party = playerData.getRoster()->getParty();
   
   int i = 0;
   for(std::vector<Character*>::const_iterator iter = party.begin(); iter != party.end() && i < PlayerData::PARTY_SIZE; ++iter, ++i)
   {
      characterModules[i].setCharacter(*iter);
   }

   for(; i < PlayerData::PARTY_SIZE; ++i)
   {
      characterModules[i].setCharacter(NULL);
   }
}

void HomePane::action(const gcn::ActionEvent& event)
{
   for(int i = 0; i < PlayerData::PARTY_SIZE; ++i)
   {
      if(&(characterModules[i]) == event.getSource())
      {
         if(moduleSelectListener != NULL)
         {
            moduleSelectListener->moduleSelected(i, characterModules[i].getId());
         }
         
         break;
      }
   }
}

HomePane::~HomePane()
{
   delete[] characterModules;
}
