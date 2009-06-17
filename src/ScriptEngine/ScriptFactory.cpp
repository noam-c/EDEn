#include "ScriptFactory.h"
#include "FileScript.h"
#include "NPCScript.h"

const std::string ScriptFactory::EXTENSION = ".lua";
const std::string ScriptFactory::PATHS[] = { "data/scripts/chapters/", "data/scripts/maps/", "data/scripts/npcs/" };

std::string ScriptFactory::getPath(std::string name, ScriptType type)
{  return PATHS[type] + name + EXTENSION;
}

Script* ScriptFactory::createScript(lua_State* luaVM, std::string name, ScriptType type)
{  std::string path = getPath(name, type);

   if(type == NPC_SCRIPT)
   {  return new NPCScript(luaVM, path);
   }

   return new FileScript(luaVM, path);
}

NPCScript* ScriptFactory::getNPCScript(lua_State* luaVM, std::string regionName, std::string mapName, std::string npcName)
{  std::string scriptName = regionName + '/' + mapName + '/' + npcName;
   return static_cast<NPCScript*>(createScript(luaVM, scriptName, NPC_SCRIPT));
}

Script* ScriptFactory::getMapScript(lua_State* luaVM, std::string regionName, std::string mapName)
{  std::string scriptName = regionName + '/' + mapName;
   return createScript(luaVM, scriptName, MAP_SCRIPT);
}

Script* ScriptFactory::getChapterScript(lua_State* luaVM, std::string name)
{   return createScript(luaVM, name, CHAPTER_SCRIPT);
}
