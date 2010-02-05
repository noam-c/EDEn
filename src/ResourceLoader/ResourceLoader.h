#ifndef __RESOURCE_LOADER_H_
#define __RESOURCE_LOADER_H_

#include <map>
#include <string>
#include "ResourceKey.h"
#include "SDL_mixer.h"

class Resource;
class Music;
class Sound;
class Region;
class Tileset;
class Spritesheet;

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
   {
      /** Sound effects to be played during gameplay */
      SOUND,
      /** Locations and maps navigated via the TileEngine */
      REGION,
      /** Sets of tiles that may be drawn via the TileEngine */
      TILESET,
      /** Pieces of music to be played in the game background */
      MUSIC,
      /** Sheets of sprites that can be drawn on screen to represent moving objects */
      SPRITESHEET,
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
    * Create a resource specified by the given unique key-type pair, and load
    * its data from file. If there is a problem loading the data, the
    * uninitialized resource is returned and acts as a stub.
    *
    * @param name The name of the resource to be loaded.
    * @param type The ResourceType of the resource to be loaded.
    *
    * @return A pointer to the created resource.
    */
   static Resource* loadNewResource(ResourceKey name, ResourceType type);

   /**
    * Attempts to initialize the resource by loading its data from file.
    * If an exception occurs, the resource remains in uninitialized state,
    * and the method outputs an error to debug.
    *
    * @param resource The pointer to the resource to initialize.
    * @param name The name of the resource.
    * @param type The type of resource.
    */
   static void tryInitialize(Resource* resource, ResourceKey name, ResourceType type);

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
    */
   static Resource* getResource(ResourceKey name, ResourceType type);

   public:
      /**
       * Get a music resource with the specified filename.
       * The extension must also be specified since music
       * can appear in multiple file types.
       *
       * @return The piece of music given by the specified name.
       */
      static Music* getMusic(ResourceKey name);

      /**
       * @return The sound effect given by the specified name.
       */
      static Sound* getSound(ResourceKey name);

      /**
       * @return The tileset given by the specified name.
       */
      static Tileset* getTileset(ResourceKey name);

      /**
       * @return The spritesheet given by the specified name.
       */
      static Spritesheet* getSpritesheet(ResourceKey name);

      /**
       * @return The region data given by the specified name.
       */
      static Region* getRegion(ResourceKey name);

      /**
       * Free all of the memory taken up by the resources, deleting all the
       * Resources along the way.
       */
      static void freeAll();
};

#endif
