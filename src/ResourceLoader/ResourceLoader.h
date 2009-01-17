#ifndef __RESOURCE_LOADER_H_
#define __RESOURCE_LOADER_H_

#include <map>
#include <string>
#include "ResourceException.h"
#include "SDL_mixer.h"

class Region;
class Resource;
class Tileset;

/**
 * Responsible for loading (eventually caching and even preloading!) data resources such as
 * tilesets, maps, music, and anything else loaded from the file system for use in the game.
 *
 * \todo This class should be a singleton.
 *
 * @author Noam Chitayat
 */
class ResourceLoader
{  static const char* PATHS[];
   static const char* EXTENSIONS[];

   typedef std::string Key;

   enum ResourceType
   { MUSIC,
     SOUND,
     TILESET,
     REGION
   };

   static std::map<Key, Resource*> resources;

   static void load(Key name, ResourceType type);
   static char* getPath(Key name, ResourceType type);
   static char* getRegionPath(Key name);
   static Resource* getResource(Key name, ResourceType type) throw(ResourceException);

   public:
      static Mix_Music* getMusic(Key name) throw(ResourceException);
      static Mix_Chunk* getSound(Key name) throw(ResourceException);
      static Tileset* getTileset(Key name) throw(ResourceException);
      static Region* getRegion(Key name) throw(ResourceException);

      static char* getRegionFolder(Key name);
      static void freeAll();
};

#endif
