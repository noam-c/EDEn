/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ScriptFactory.h"

#include <sstream>
#include <type_traits>

#include "FileScript.h"
#include "UsableScript.h"
#include "Item.h"
#include "Skill.h"
#include "NPCScript.h"
#include "NPC.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_SCRIPT_ENG

const std::string ScriptFactory::EXTENSION = ".lua";
const std::string ScriptFactory::PATHS[] = { "data/scripts/chapters/", "data/scripts/maps/", "data/scripts/npcs/", "data/scripts/items/", "data/scripts/skills/" };

std::string ScriptFactory::getPath(const std::string& name, ScriptType type)
{
   static_assert(std::is_integral<std::underlying_type<ScriptType>::type>::value, "ScriptType has to be integral since it is used as an array subscript.");
   return PATHS[static_cast<int>(type)] + name + EXTENSION;
}

std::shared_ptr<Script> ScriptFactory::createScript(lua_State* luaVM, const std::string& name, ScriptType type)
{
   auto fileScript = std::make_shared<FileScript>(luaVM, ScriptFactory::getPath(name, type));
   if(fileScript->initialize())
   {
      return fileScript;
   }

   return nullptr;
}

std::shared_ptr<NPCScript> ScriptFactory::createNPCCoroutine(lua_State* luaVM, NPC* npc, const std::string& regionName, const std::string& mapName)
{
   std::string scriptName = regionName + '/' + mapName + '/' + npc->getName();
   return std::make_shared<NPCScript>(luaVM, ScriptFactory::getPath(scriptName, ScriptType::NPC_SCRIPT), npc);
}

std::shared_ptr<UsableScript> ScriptFactory::getItemScript(lua_State* luaVM, const Item& item)
{
   std::ostringstream itemIdBuffer;
   itemIdBuffer << item.getId();

   return std::make_shared<UsableScript>(luaVM, ScriptFactory::getPath(itemIdBuffer.str(), ScriptType::ITEM_SCRIPT), item);
}

std::shared_ptr<UsableScript> ScriptFactory::getSkillScript(lua_State* luaVM, const Skill& skill)
{
   std::ostringstream skillIdBuffer;
   skillIdBuffer << skill.getId();

   return std::make_shared<UsableScript>(luaVM, ScriptFactory::getPath(skillIdBuffer.str(), ScriptType::SKILL_SCRIPT), skill);
}

std::shared_ptr<Script> ScriptFactory::getMapScript(lua_State* luaVM, const std::string& regionName, const std::string& mapName)
{
   std::string scriptName = regionName + '/' + mapName;
   return ScriptFactory::createScript(luaVM, scriptName, ScriptType::MAP_SCRIPT);
}

std::shared_ptr<Script> ScriptFactory::getChapterScript(lua_State* luaVM, const std::string& name)
{
    return ScriptFactory::createScript(luaVM, name, ScriptType::CHAPTER_SCRIPT);
}
