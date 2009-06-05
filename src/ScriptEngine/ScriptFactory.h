#ifndef __SCRIPT_FACTORY_H_
#define __SCRIPT_FACTORY_H_

#include <string>

class Script;
struct lua_State;

/**
 * \todo Document.
 */
class ScriptFactory
{  /**
    * The ScriptFactory handles creation of various kinds of scripts.
    * This enum contains the types of resources available.
    */
   enum ScriptType
   {  /** Scripts to be called when chapters are loaded */
      CHAPTER_SCRIPT,
      /** Scripts to be called when maps are loaded */
      MAP_SCRIPT,
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
   static Script* createScript(lua_State* luaVM, std::string name, ScriptType type);

   /**
    * Get the path to a certain resource based on its name and type.
    *
    * @param name The name of the script.
    * @param type The type of script.
    *
    * @return A relative path to the script "name"
    */
   static std::string getPath(std::string name, ScriptType type);

   public:
      /**
       * @param luaVM The Lua VM to be used to load the script if it is not cached
       * @param regionName The name of the region containing the map
       * @param mapName The name of the map associated with the script
       *
       * @return The map script given by the specified region-map pairing
       */
      static Script* getMapScript(lua_State* luaVM, std::string regionName, std::string mapName);

      /**
       * @param luaVM The Lua VM to be used to load the script if it is not cached
       * @param name The name of the chapter to load the intro script for
       *
       * @return The chapter script given by the specified chapter name
       */
      static Script* getChapterScript(lua_State* luaVM, std::string name);
};

#endif
