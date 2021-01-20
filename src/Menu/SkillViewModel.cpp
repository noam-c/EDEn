/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "SkillViewModel.h"
#include "PlayerData.h"
#include "Metadata.h"
#include "Character.h"
#include "Skill.h"
#include "SkillMenu.h"

#include <sstream>
#include <string>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_MENU

const Rml::Core::String SkillViewModel::UnknownSkillIconPath("data/images/icons/I_Rock01.png");

SkillViewModel::SkillViewModel(SkillMenu& skillMenu, const Metadata& metadata) :
   Rml::Controls::DataSource("skillViewModel"),
   m_skillMenu(skillMenu),
   m_metadata(metadata)
{
}

UsableId SkillViewModel::getSkillId(int rowIndex) const
{
   Character* selectedCharacter = m_skillMenu.getSelectedCharacter();
   if(selectedCharacter == nullptr)
   {
      return 0;
   }

   const SkillList& skillList = selectedCharacter->getSkillList();
   return skillList[rowIndex];
}

std::string SkillViewModel::getCurrentCharacterId() const
{
   Character* selectedCharacter = m_skillMenu.getSelectedCharacter();
   if(selectedCharacter != nullptr)
   {
      return selectedCharacter->getId();
   }

   return "";
}

void SkillViewModel::GetRow(Rml::Core::StringList& row,
      const Rml::Core::String& table, int row_index,
      const Rml::Core::StringList& columns)
{
   Character* selectedCharacter = m_skillMenu.getSelectedCharacter();
   if(selectedCharacter == nullptr)
   {
      return;
   }

   if (table == "skills")
   {
      const SkillList& skillList = selectedCharacter->getSkillList();
      for (int i = 0; i < columns.size(); ++i)
      {
         const UsableId skillId = skillList[row_index];
         const Skill* rowSkill = m_metadata.getSkill(skillId);
         if (columns[i] == "name")
         {
            if(rowSkill == nullptr)
            {
               row.push_back(std::string("Unknown skill: ").append(std::to_string(skillId)));
            }
            else
            {
               row.emplace_back(rowSkill->getName());
            }
         }
         else if (columns[i] == "icon")
         {
            if(rowSkill == nullptr)
            {
               row.push_back(SkillViewModel::UnknownSkillIconPath);
            }
            else
            {
               row.emplace_back(rowSkill->getIconPath().c_str());
            }
         }
      }
   }
}

int SkillViewModel::GetNumRows(const Rml::Core::String& table)
{
   Character* selectedCharacter = m_skillMenu.getSelectedCharacter();
   if(selectedCharacter != nullptr && table == "skills")
   {
      return selectedCharacter->getSkillList().size();
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
