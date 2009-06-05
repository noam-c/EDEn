#include "ScriptFactory.h"
#include "FileScript.h"

const std::string ScriptFactory::EXTENSION = ".lua";
const std::string ScriptFactory::PATHS[] = { "data/scripts/chapters/", "data/scripts/maps/" };

std::string ScriptFactory::getPath(std::string name, ScriptType type)
{  return PATHS[type] + name + EXTENSION;
}

Script* ScriptFactory::createScript(lua_State* luaVM, std::string name, ScriptType type)
{  std::string path = getPath(name, type);
   return new FileScript(luaVM, path);
}

Script* ScriptFactory::getMapScript(lua_State* luaVM, std::string regionName, std::string mapName)
{   std::string scriptName = regionName + '/' + mapName;
    return static_cast<FileScript*>(createScript(luaVM, scriptName, MAP_SCRIPT));
}

Script* ScriptFactory::getChapterScript(lua_State* luaVM, std::string name)
{   return static_cast<FileScript*>(createScript(luaVM, name, CHAPTER_SCRIPT));
}
