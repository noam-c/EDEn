#include "SaveGameModule.h"

#include "PlayerData.h"
#include "Character.h"
#include <sstream>
#include "Container.h"
#include "Label.h"
#include "Icon.h"

SaveGameModule::SaveGameModule(PlayerData& playerData) : playerData(playerData)
{
   setNumberOfColumns(PlayerData::PARTY_SIZE);
   setHorizontalSpacing(10);
   setPadding(5, 5, 5, 5);

   CharacterList party = playerData.getParty();
   for(CharacterList::iterator iter = party.begin(); iter != party.end(); ++iter)
   {
      edwt::Icon* characterPicture = new edwt::Icon(iter->second->getPortraitPath());

      add(characterPicture);
      characterPortraits.push_back(characterPicture);
   }
   
   adjustContent();
   addMouseListener(this);
   setOpaque(false);
}

void SaveGameModule::mouseClicked(gcn::MouseEvent& mouseEvent)
{
}

SaveGameModule::~SaveGameModule()
{
   for(std::vector<edwt::Icon*>::iterator iter = characterPortraits.begin(); iter != characterPortraits.end(); ++iter)
   {
      delete *iter;
   }
}
