/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Metadata.h"
#include "Item.h"
#include "Skill.h"
#include "json.h"
#include <fstream>

#include "DebugUtils.h"

const int debugFlag = DEBUG_PLAYER;
const char* ITEM_DATA_PATH = "data/metadata/items.edb";
const char* SKILL_DATA_PATH = "data/metadata/skills.edb";

Metadata::Metadata(GameContext& gameContext)
{
   loadItemMetadata();
   loadSkillMetadata();
}

Metadata::~Metadata()
{
   for(std::map<ItemId, Item*>::iterator iter = items.begin(); iter != items.end(); ++iter)
   {
      delete iter->second;
   }

   for(std::map<SkillId, Skill*>::iterator iter = skills.begin(); iter != skills.end(); ++iter)
   {
      delete iter->second;
   }
}

Json::Value Metadata::loadMetadataTable(const char* filePath)
{
   Json::Reader reader;
   DEBUG("Loading metadata file %s", filePath);

   std::ifstream input(filePath);
   if(!input)
   {
      T_T("Failed to open metadata file for reading.");
   }

   Json::Value jsonRoot;
   if(!reader.parse(input, jsonRoot))
   {
      DEBUG("Failed to parse metadata: %s", reader.getFormattedErrorMessages().c_str());
      T_T("Requested metadata database is corrupt.");
   }

   Json::Value& dataList = jsonRoot["data"];

   if(!dataList.isArray())
   {
      T_T("Failed to parse metadata: Expected \"data\" to be an array.");
   }

   DEBUG("Loaded metadata file %s", filePath);
   return dataList;
}

void Metadata::loadItemMetadata()
{
   Json::Value itemList = loadMetadataTable(ITEM_DATA_PATH);
   int numItems = itemList.size();
   for(int i = 0; i < numItems; ++i)
   {
      int id = itemList[i]["id"].asInt();
      items[id] = new Item(itemList[i]);
      DEBUG("Loaded item ID %d", id);
   } 

   DEBUG("Item data loaded.");
}

void Metadata::loadSkillMetadata()
{
   Json::Value skillList = loadMetadataTable(SKILL_DATA_PATH);
   int numSkills = skillList.size();
   for(int i = 0; i < numSkills; ++i)
   {
      int id = skillList[i]["id"].asInt();
      skills[id] = new Skill(skillList[i]);
      DEBUG("Loaded skill ID %d", id);
   }

   DEBUG("Skill data loaded.");
}

Item* Metadata::getItem(ItemId key) const
{
   std::map<ItemId, Item*>::const_iterator itemIterator = items.find(key);
   if(itemIterator != items.end())
   {
      return itemIterator->second;
   }

   return NULL;
}

Skill* Metadata::getSkill(SkillId key) const
{
   std::map<SkillId, Skill*>::const_iterator skillIterator = skills.find(key);
   if(skillIterator != skills.end())
   {
      return skillIterator->second;
   }

   return NULL;
}
