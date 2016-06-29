/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ResourceLoader.h"

#include "EnumUtils.h"

#include "Music.h"
#include "Region.h"
#include "Sound.h"
#include "Spritesheet.h"
#include "Tileset.h"

#include <fstream>

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_RES_LOAD

const std::string ResourceLoader::PATHS[] = {"data/sounds/", "data/regions/", "data/tilesets/", "data/music/", "data/sprites/"};
const std::string ResourceLoader::EXTENSIONS[] = {".wav", "/", ".tsx", "", ""};

std::map<ResourceKey, std::shared_ptr<Resource>> ResourceLoader::resources;

std::string ResourceLoader::getPath(ResourceKey name, ResourceType type)
{
   const auto typeIndex = EnumUtils::toNumber(type);
   return PATHS[typeIndex] + name + EXTENSIONS[typeIndex];
}

std::shared_ptr<Resource> ResourceLoader::loadNewResource(ResourceKey name, ResourceType type)
{
   // Construct a new resource instance based on the resource type
   std::shared_ptr<Resource> newResource;
   switch(type)
   {
      case ResourceType::MUSIC:
      {
         // Create a resource to hold a song
         newResource = std::make_shared<Music>(name);
         break;
      }
      case ResourceType::SOUND:
      {
         // Create a resource to hold a sound effect
         newResource = std::make_shared<Sound>(name);
         break;
      }
      case ResourceType::TILESET:
      {
         // Create a resource to hold a tile set
         newResource = std::make_shared<Tileset>(name);
         break;
      }
      case ResourceType::REGION:
      {
         // Create a resource to hold a region
         newResource = std::make_shared<Region>(name);
         break;
      }
      case ResourceType::SPRITESHEET:
      {
         // Create a resource to hold a spritesheet
         newResource = std::make_shared<Spritesheet>(name);
         break;
      }
   }

   // Try to load the data for this resource from file
   tryInitialize(newResource, name, type);

   // Place the new resource into the resource map and return it
   resources[name] = newResource;
   return newResource;
}

void ResourceLoader::tryInitialize(const std::shared_ptr<Resource>& resource, ResourceKey name, ResourceType type)
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

std::shared_ptr<Resource> ResourceLoader::getResource(ResourceKey name, ResourceType type)
{
   std::shared_ptr<Resource> resource;

   auto resourceIter = resources.find(name);
   if(resourceIter == resources.end())
   {
      // If the resource is not already in the resource map, it is not
      // currently being cached. Construct a resource and load the data.
      resource = loadNewResource(name, type);
   }
   else
   {
      // If the resource is cached, check that it is already initialized.
      resource = resourceIter->second;
      if(!resource->isInitialized())
      {
         // If it is not (because of a prior failure to initialize),
         // make another attempt to load the resource.
         tryInitialize(resource, name, type);
      }
   }

   return resource;
}

std::shared_ptr<Music> ResourceLoader::getMusic(ResourceKey name)
{
   return std::static_pointer_cast<Music>(getResource(name, ResourceType::MUSIC));
}

std::shared_ptr<Sound> ResourceLoader::getSound(ResourceKey name)
{
   return std::static_pointer_cast<Sound>(getResource(name, ResourceType::SOUND));
}

std::shared_ptr<Tileset> ResourceLoader::getTileset(ResourceKey name)
{
   return std::static_pointer_cast<Tileset>(getResource(name, ResourceType::TILESET));
}

std::shared_ptr<Region> ResourceLoader::getRegion(ResourceKey name)
{
   return std::static_pointer_cast<Region>(getResource(name, ResourceType::REGION));
}

std::shared_ptr<Spritesheet> ResourceLoader::getSpritesheet(ResourceKey name)
{
   return std::static_pointer_cast<Spritesheet>(getResource(name, ResourceType::SPRITESHEET));
}

void ResourceLoader::freeAll()
{
   resources.clear();
}
