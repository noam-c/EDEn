#ifndef __RESOURCE_LOADER_H_
#define __RESOURCE_LOADER_H_

#include <map>
#include <string>
#include "ResourceKey.h"
#include "ResourceException.h"
#include "SDL_mixer.h"

class Resource;
class Music;
class Sound;
class Region;
class Tileset;

/**
 * Responsible for loading (eventually caching and even preloading!) data resources such as
 * tilesets, maps, music, and anything else loaded from the file system for use in the game.
 *
 * @author Noam Chitayat
 */
class ResourceLoader
{
   /**
    * The ResourceLoader handles loading and caching of multiple
    * types of resources. This enum contains the types of resources
    * available.
    */
   enum ResourceType
   { /** Sound effects to be played during gameplay */
     SOUND,
     /** Sets of tiles that may be drawn via the TileEngine */
     TILESET,
     /** Locations and maps navigated via the TileEngine */
     REGION,
     /** Pieces of music to be played in the game background */
     MUSIC,
   };

   /** 
    *  A list of paths to various kinds of resources, in the same order as the
    *  ResourceType enum.
    */
   static const std::string PATHS[];

   /** 
    *  A list of file extensions for various kinds of resources, in the same
    *  order as the ResourceType enum.
    */
   static const std::string EXTENSIONS[];

   /** A map to hold all the currently loaded resources, organized by key */
   static std::map<ResourceKey, Resource*> resources;

   /**
    * Load in a resource specified by the given unique key-type pair.
    *
    * @param name The name of the resource to be loaded.
    * @param type The ResourceType of the resource to be loaded.
    */
   static void load(ResourceKey name, ResourceType type);

   /**
    * Get the path to a certain resource based on its name and type.
    *
    * @param name The name of the resource.
    * @param type The type of resource.
    *
    * @return A relative path to the resource "name"
    */
   static std::string getPath(ResourceKey name, ResourceType type);

   /**
    * Get a resource of a certain name and type. If the resource is not cached
    * already, then it will be loaded first.
    *
    * @param name The name of the resource.
    * @param type The type of resource.
    *
    * @return A pointer to the resource requested.
    * @throws ResourceException If the resource cannot be found or loaded
    *         (for example, file not found or malformed resource).
    */
   static Resource* getResource(ResourceKey name, ResourceType type) throw(ResourceException);

   public:
      /**
       * Get a music resource with the specified filename.
       * The extension must also be specified since music
       * can appear in multiple file types.
       *
       * @return The piece of music given by the specified name.
       * @throws ResourceException If the music resource cannot be found or
       * loaded.
       */
      static Music* getMusic(ResourceKey name) throw(ResourceException);

      /**
       * @return The sound effect given by the specified name.
       * @throws ResourceException If the sound resource cannot be found or
       * loaded.
       */
      static Sound* getSound(ResourceKey name) throw(ResourceException);

      /**
       * @return The tileset given by the specified name.
       * @throws ResourceException If the tileset resource cannot be found or
       * loaded.
       */
      static Tileset* getTileset(ResourceKey name) throw(ResourceException);

      /**
       * @return The region data given by the specified name.
       * @throws ResourceException If the region resource cannot be found or
       * loaded.
       */
      static Region* getRegion(ResourceKey name) throw(ResourceException);

      /**
       * Free all of the memory taken up by the resources, deleting all the
       * Resources along the way.
       */
      static void freeAll();
};

#endif
