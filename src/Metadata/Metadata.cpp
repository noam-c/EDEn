/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Metadata.h"
#include "json.h"
#include <fstream>
#include <tuple>
#include <utility>

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_PLAYER
const char* ITEM_DATA_PATH = "data/metadata/items.edb";
const char* SKILL_DATA_PATH = "data/metadata/skills.edb";

Metadata::Metadata(ScriptEngine& scriptEngine) :
   m_scriptEngine(scriptEngine)
{
   loadItemMetadata();
   loadSkillMetadata();
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
      m_items.emplace(std::piecewise_construct,
                      std::forward_as_tuple(id),
                      std::forward_as_tuple(itemList[i]));
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
      m_skills.emplace(std::piecewise_construct,
                       std::forward_as_tuple(id),
                       std::forward_as_tuple(skillList[i]));
      DEBUG("Loaded skill ID %d", id);
   }

   DEBUG("Skill data loaded.");
}

const Item* Metadata::getItem(UsableId key) const
{
   const auto& itemIterator = m_items.find(key);
   if(itemIterator != m_items.end())
   {
      return &(itemIterator->second);
   }

   return nullptr;
}

const Skill* Metadata::getSkill(UsableId key) const
{
   const auto& skillIterator = m_skills.find(key);
   if(skillIterator != m_skills.end())
   {
      return &(skillIterator->second);
   }

   return nullptr;
}

bool Metadata::useItem(UsableId key, const GameStateType stateType)
{
   const auto& itemIterator = m_items.find(key);
   if(itemIterator != m_items.end())
   {
      itemIterator->second.use(m_scriptEngine, stateType);
      return true;
   }

   DEBUG("Attempted to use item %d that does not exist", key);
   return false;
}

bool Metadata::useSkill(UsableId key, const GameStateType stateType, Character* usingCharacter)
{
   const auto& skillIterator = m_skills.find(key);
   if(skillIterator != m_skills.end())
   {
      skillIterator->second.use(m_scriptEngine, stateType, usingCharacter);
      return true;
   }

   DEBUG("Attempted to use skill %d that does not exist", key);
   return false;
}
