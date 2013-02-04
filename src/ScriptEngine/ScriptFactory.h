/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef SCRIPT_FACTORY_H
#define SCRIPT_FACTORY_H

#include <string>

class Script;
typedef class Usable Item;
class UsableScript;
class NPC;
class NPCScript;
struct lua_State;

/**
 * The ScriptFactory is a factory class that loads FileScripts and NPCScripts
 * based on information supplied by the ScriptEngine. This class is used to
 * abstract the details of loading in new Script files.
 *
 * Unlike the ResourceLoader, the ScriptFactory does not cache or manage loaded
 * Script objects. The Scripts are managed outside of the ScriptFactory and must
 * be cleaned up by the script initializer.
 *
 * @author Noam Chitayat
 */
class ScriptFactory
{
   /**
    * The ScriptFactory handles creation of various kinds of scripts.
    * This enum contains the types of resources available.
    */
   enum ScriptType
   {
      /** Scripts to be called when chapters are loaded */
      CHAPTER_SCRIPT,
      /** Scripts to be called when maps are loaded */
      MAP_SCRIPT,
      /** Scripts containing instructions for NPC behaviour */
      NPC_SCRIPT,
      /** Scripts containing instructions for item behaviour */
      ITEM_SCRIPT,
   };

   /** 
    *  The file extension used for scripts.
    */
   static const std::string EXTENSION;

   /** 
    * A list of paths to various kinds of scripts, in the same order as the
    * ScriptType enum.
    */
   static const std::string PATHS[];

   /**
    * Load a script specified by the given name.
    *
    * @param luaVM The Lua VM used to create the script.
    * @param name The name of the script to be loaded.
    * @param type The ScriptType of the script to be loaded.
    */
   static Script* createScript(lua_State* luaVM, const std::string& name, ScriptType type);

   /**
    * Get the path to a certain resource based on its name and type.
    *
    * @param name The name of the script.
    * @param type The type of script.
    *
    * @return A relative path to the script "name"
    */
   static std::string getPath(const std::string& name, ScriptType type);

   public:
      /**
       * @param luaVM The Lua VM to be used to load the script
       * @param npc The NPC to retrieve scripts for.
       * @param regionName The name of the region containing the map
       * @param mapName The name of the map containing the NPC
       *
       * @return The NPC script associated with the NPC requested
       */
      static NPCScript* createNPCCoroutine(lua_State* luaVM, NPC* npc, const std::string& regionName, const std::string& mapName);

      /**
       * @param luaVM The Lua VM to be used to load the script
       * @param item The item to load the script for
       *
       * @return The item script given by the specified item ID
       */
      static UsableScript* getItemScript(lua_State* luaVM, const Item& item);

      /**
       * @param luaVM The Lua VM to be used to load the script
       * @param regionName The name of the region containing the map
       * @param mapName The name of the map associated with the script
       *
       * @return The map script given by the specified region-map pairing
       */
      static Script* getMapScript(lua_State* luaVM, const std::string& regionName, const std::string& mapName);

      /**
       * @param luaVM The Lua VM to be used to load the script
       * @param name The name of the chapter to load the intro script for
       *
       * @return The chapter script given by the specified chapter name
       */
      static Script* getChapterScript(lua_State* luaVM, const std::string& name);
};

#endif
