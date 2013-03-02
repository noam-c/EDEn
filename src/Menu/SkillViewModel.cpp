/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "SkillViewModel.h"
#include "PlayerData.h"
#include "GameContext.h"
#include "PlayerData.h"
#include "Character.h"
#include "Skill.h"

#include "dirent.h"
#include <sstream>
#include <string>

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

const Rocket::Core::String SkillViewModel::UnknownSkillIconPath("data/images/icons/I_Rock01.png");

SkillViewModel::SkillViewModel(GameContext& gameContext, PlayerData& playerData) :
      Rocket::Controls::DataSource("skillViewModel"),
      gameContext(gameContext),
      playerData(playerData),
      selectedCharacter(NULL)
{
}

SkillViewModel::~SkillViewModel()
{
}

void SkillViewModel::setCharacter(int characterIndex)
{
   CharacterRoster* roster = playerData.getRoster();

   selectedCharacter = roster != NULL ?
      selectedCharacter = roster->getParty()[characterIndex] :
      NULL;


   NotifyRowChange("skills");
}

void SkillViewModel::useSkill(int rowIndex)
{
   if(selectedCharacter == NULL)
   {
      return;
   }

   const SkillList& skillList = selectedCharacter->getSkillList();
   const UsableId skillId = skillList[rowIndex];
   Skill* skill = gameContext.getSkill(skillId);
   if(skill == NULL)
   {
      DEBUG("Tried to use bad skill with ID: %d.", skillId);
   }
   else
   {
      skill->use(gameContext, selectedCharacter);
      NotifyRowChange("skills", rowIndex, 1);
   }
}

UsableId SkillViewModel::getSkillId(int rowIndex) const
{
   if(selectedCharacter == NULL)
   {
      return 0;
   }

   const SkillList& skillList = selectedCharacter->getSkillList();
   return skillList[rowIndex];
}

std::string SkillViewModel::getCurrentCharacterId() const
{
   if(selectedCharacter != NULL)
   {
      return selectedCharacter->getId();
   }

   return "";
}

void SkillViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   if(selectedCharacter == NULL)
   {
      return;
   }

   if (table == "skills")
   {
      const SkillList& skillList = selectedCharacter->getSkillList();
      for (int i = 0; i < columns.size(); ++i)
      {
         const UsableId skillId = skillList[row_index];
         const Skill* rowSkill = gameContext.getSkill(skillId);
         if (columns[i] == "name")
         {
            if(rowSkill == NULL)
            {
               row.push_back(Rocket::Core::String(13, "Unknown %d", skillId));
            }
            else
            {
               row.push_back(rowSkill->getName().c_str());
            }
         }
         else if (columns[i] == "icon")
         {
            if(rowSkill == NULL)
            {
               row.push_back(SkillViewModel::UnknownSkillIconPath);
            }
            else
            {
               row.push_back(rowSkill->getIconPath().c_str());
            }
         }
      }
   }
}

int SkillViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if(selectedCharacter != NULL && table == "skills")
   {
      return selectedCharacter->getSkillList().size();
   }

   return 0;
}
