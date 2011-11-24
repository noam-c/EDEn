/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "ScriptFactory.h"
#include "FileScript.h"
#include "NPCScript.h"
#include <cstdarg>

const std::string ScriptFactory::EXTENSION = ".lua";
const std::string ScriptFactory::PATHS[] = { "data/scripts/chapters/", "data/scripts/maps/", "data/scripts/npcs/" };

std::string ScriptFactory::getPath(const std::string& name, ScriptType type)
{
   return PATHS[type] + name + EXTENSION;
}

Script* ScriptFactory::createScript(lua_State* luaVM, const std::string& name, ScriptType type, ...)
{
   std::string path = getPath(name, type);

   if(type == NPC_SCRIPT)
   {
      va_list args;
      va_start(args, type);
      NPC* npcToBind = va_arg(args, NPC*);
      va_end(args);

      return new NPCScript(luaVM, path, npcToBind);
   }

   return new FileScript(luaVM, path);
}

NPCScript* ScriptFactory::getNPCScript(lua_State* luaVM, NPC* npc, const std::string& regionName, const std::string& mapName, const std::string& npcName)
{
   std::string scriptName = regionName + '/' + mapName + '/' + npcName;
   return static_cast<NPCScript*>(createScript(luaVM, scriptName, NPC_SCRIPT, npc));
}

Script* ScriptFactory::getMapScript(lua_State* luaVM, const std::string& regionName, const std::string& mapName)
{
   std::string scriptName = regionName + '/' + mapName;
   return createScript(luaVM, scriptName, MAP_SCRIPT);
}

Script* ScriptFactory::getChapterScript(lua_State* luaVM, const std::string& name)
{
    return createScript(luaVM, name, CHAPTER_SCRIPT);
}
