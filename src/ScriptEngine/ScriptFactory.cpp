/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ScriptFactory.h"

#include <sstream>

#include "FileScript.h"
#include "UsableScript.h"
#include "Item.h"
#include "NPCScript.h"
#include "NPC.h"

const std::string ScriptFactory::EXTENSION = ".lua";
const std::string ScriptFactory::PATHS[] = { "data/scripts/chapters/", "data/scripts/maps/", "data/scripts/npcs/", "data/scripts/items/" };

std::string ScriptFactory::getPath(const std::string& name, ScriptType type)
{
   return PATHS[type] + name + EXTENSION;
}

Script* ScriptFactory::createScript(lua_State* luaVM, const std::string& name, ScriptType type)
{
   return new FileScript(luaVM, ScriptFactory::getPath(name, type));
}

NPCScript* ScriptFactory::createNPCCoroutine(lua_State* luaVM, NPC* npc, const std::string& regionName, const std::string& mapName)
{
   std::string scriptName = regionName + '/' + mapName + '/' + npc->getName();
   return new NPCScript(luaVM, ScriptFactory::getPath(scriptName, NPC_SCRIPT), npc);
}

UsableScript* ScriptFactory::getItemScript(lua_State* luaVM, const Item& item)
{
   std::ostringstream itemIdBuffer;
   itemIdBuffer << item.getId();

   return new UsableScript(luaVM, ScriptFactory::getPath(itemIdBuffer.str(), ITEM_SCRIPT), item);
}

Script* ScriptFactory::getMapScript(lua_State* luaVM, const std::string& regionName, const std::string& mapName)
{
   std::string scriptName = regionName + '/' + mapName;
   return ScriptFactory::createScript(luaVM, scriptName, MAP_SCRIPT);
}

Script* ScriptFactory::getChapterScript(lua_State* luaVM, const std::string& name)
{
    return ScriptFactory::createScript(luaVM, name, CHAPTER_SCRIPT);
}
