/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "SkillViewModel.h"
#include "PlayerData.h"
#include "Metadata.h"
#include "Character.h"
#include "Skill.h"
#include "SkillMenu.h"

#include "dirent.h"
#include <sstream>
#include <string>

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

const Rocket::Core::String SkillViewModel::UnknownSkillIconPath("data/images/icons/I_Rock01.png");

SkillViewModel::SkillViewModel(SkillMenu& skillMenu, const Metadata& metadata) :
      Rocket::Controls::DataSource("skillViewModel"),
      m_skillMenu(skillMenu),
      m_metadata(metadata),
      m_selectedCharacter(NULL)
{
}

SkillViewModel::~SkillViewModel()
{
}

UsableId SkillViewModel::getSkillId(int rowIndex) const
{
   if(m_selectedCharacter == NULL)
   {
      return 0;
   }

   const SkillList& skillList = m_selectedCharacter->getSkillList();
   return skillList[rowIndex];
}

std::string SkillViewModel::getCurrentCharacterId() const
{
   if(m_selectedCharacter != NULL)
   {
      return m_selectedCharacter->getId();
   }

   return "";
}

void SkillViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   if(m_selectedCharacter == NULL)
   {
      return;
   }

   if (table == "skills")
   {
      const SkillList& skillList = m_selectedCharacter->getSkillList();
      for (int i = 0; i < columns.size(); ++i)
      {
         const UsableId skillId = skillList[row_index];
         const Skill* rowSkill = m_metadata.getSkill(skillId);
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
   if(m_selectedCharacter != NULL && table == "skills")
   {
      return m_selectedCharacter->getSkillList().size();
   }

   return 0;
}

void SkillViewModel::refresh()
{
   NotifyRowChange("skills");
}

void SkillViewModel::refresh(int rowIndex)
{
   NotifyRowChange("skills", rowIndex, 1);
}
