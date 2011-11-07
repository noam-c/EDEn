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
   CharacterList party = playerData.getParty();
   
   int i = 0;
   for(CharacterList::iterator iter = party.begin(); iter != party.end(); ++iter, ++i)
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