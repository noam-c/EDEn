/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ResourceLoader.h"

#include "Music.h"
#include "Sound.h"
#include "Tileset.h"
#include "Region.h"
#include "Spritesheet.h"

#include <fstream>

#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

const std::string ResourceLoader::PATHS[] = {"data/sounds/", "data/regions/", "data/tilesets/", "data/music/", "data/sprites/"};
const std::string ResourceLoader::EXTENSIONS[] = {".wav", "/", ".tsx", "", ""};

std::map<ResourceKey, Resource*> ResourceLoader::resources;

std::string ResourceLoader::getPath(ResourceKey name, ResourceType type)
{
   return PATHS[type] + name + EXTENSIONS[type];
}

Resource* ResourceLoader::loadNewResource(ResourceKey name, ResourceType type)
{
   // Construct a new resource instance based on the resource type
   Resource* newResource = NULL;
   switch(type)
   {
      case MUSIC:
      {
         // Create a resource to hold a song
         newResource = new Music(name);
         break;
      }
      case SOUND:
      {
         // Create a resource to hold a sound effect
         newResource = new Sound(name);
         break;
      }
      case TILESET:
      {
         // Create a resource to hold a tile set
         newResource = new Tileset(name);
         break;
      }
      case REGION:
      {
         // Create a resource to hold a region
         newResource = new Region(name);
         break;
      }
      case SPRITESHEET:
      {
         // Create a resource to hold a spritesheet
         newResource = new Spritesheet(name);
         break;
      }
   }

   // Try to load the data for this resource from file
   tryInitialize(newResource, name, type);

   // Place the new resource into the resource map and return it
   resources[name] = newResource;
   return newResource;
}

void ResourceLoader::tryInitialize(Resource* resource, ResourceKey name, ResourceType type)
{
   DEBUG("Trying to initialize resource %s", name.c_str());
   // Get the path to the data for this resource
   std::string path = getPath(name, type);

   try
   {
      // Attempt to load the resource from its data file
      resource->initialize(path.c_str());
      DEBUG("Resource %s initialized.", name.c_str());
   }
   catch(Exception& e)
   {
      // On failure, print to debug output and return
      DEBUG("Failed to load resource %s.\n\tReason: %s", path.c_str(), e.getMessage().c_str());
   }
}

Resource* ResourceLoader::getResource(ResourceKey name, ResourceType type)
{
   Resource* resource = NULL;

   if(resources.find(name) == resources.end())
   {
      // If the resource is not already in the resource map, it is not
      // currently being cached. Construct a resource and load the data.
      resource = loadNewResource(name, type);
   }
   else
   {
      // If the resource is cached, check that it is already initialized.
      resource = resources[name];
      if(!resource->isInitialized())
      {
         // If it is not (because of a prior failure to initialize),
         // make another attempt to load the resource.
         tryInitialize(resource, name, type);
      }
   }

   return resource;
}

Music* ResourceLoader::getMusic(ResourceKey name)
{
   return static_cast<Music*>(getResource(name, MUSIC));
}

Sound* ResourceLoader::getSound(ResourceKey name)
{
   return static_cast<Sound*>(getResource(name, SOUND));
}

Tileset* ResourceLoader::getTileset(ResourceKey name)
{
   return static_cast<Tileset*>(getResource(name, TILESET));
}

Region* ResourceLoader::getRegion(ResourceKey name)
{
   return static_cast<Region*>(getResource(name, REGION));
}

Spritesheet* ResourceLoader::getSpritesheet(ResourceKey name)
{
   return static_cast<Spritesheet*>(getResource(name, SPRITESHEET));
}

void ResourceLoader::freeAll()
{
   // Iterate through the resource map and clear out all of the cached resources
   for(std::map<ResourceKey, Resource*>::iterator i = resources.begin(); i != resources.end(); ++i)
   {
      delete (i->second);
   }

   resources.clear();
}
