#include "HomePane.h"
#include "PlayerData.h"
#include "CharacterModule.h"

#define PARTY_SIZE 4

HomePane::HomePane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
   characterModules = new CharacterModule[PARTY_SIZE];

   const int moduleHeight = rect.height / PARTY_SIZE;
   for(int i = 0; i < PARTY_SIZE; ++i)
   {
      characterModules[i].setHeight(moduleHeight);
      characterModules[i].setWidth(rect.width);

      add(characterModules + i, 0, i * moduleHeight);
   }

   refresh();
}

void HomePane::refresh()
{
   CharacterList party = playerData.getParty();
   
   int i = 0;
   for(CharacterList::iterator iter = party.begin(); iter != party.end(); ++iter, ++i)
   {
      characterModules[i].setCharacter(iter->second);
   }

   for(; i < 4; ++i)
   {
      characterModules[i].setCharacter(NULL);
   }
}

HomePane::~HomePane()
{
   delete[] characterModules;
}