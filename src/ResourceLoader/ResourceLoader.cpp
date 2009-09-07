#include "ResourceLoader.h"

#include "Music.h"
#include "Sound.h"
#include "Tileset.h"
#include "Region.h"
#include "Spritesheet.h"

#include <fstream>

#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD;

const std::string ResourceLoader::PATHS[] = {"data/sounds/", "data/tilesets/", "data/regions/", "data/music/", "data/sprites/"};
const std::string ResourceLoader::EXTENSIONS[] = {".wav", ".edt", ".edr", "", ""};

extern std::map<ResourceKey, Resource*> ResourceLoader::resources;

std::string ResourceLoader::getPath(ResourceKey name, ResourceType type)
{  return PATHS[type] + name + EXTENSIONS[type];
}

Resource* ResourceLoader::load(ResourceKey name, ResourceType type)
{  std::string path = getPath(name, type);

   Resource* newResource = NULL;
   switch(type)
   {  case MUSIC:
      {  newResource = new Music(name);
         break;
      }
      case SOUND:
      {  newResource = new Sound(name);
         break;
      }
      case TILESET:
      {  newResource = new Tileset(name);
         break;
      }
      case REGION:
      {  newResource = new Region(name);
         break;
      }
      case SPRITESHEET:
      {  newResource = new Spritesheet(name);
         break;
      }
   }

   try
   {  newResource->load(path.c_str());
      resources[name] = newResource;
   }
   catch(Exception e)
   {  /**
       * \todo Failed resource loads cause a memory leak!!!
       *       Cache zombie objects or find a new solution!
       */
      //delete newResource;
      DEBUG("Failed to load resource %s.\n\tReason: %s", path.c_str(), e.what());
   }

   return newResource;
}

Resource* ResourceLoader::getResource(ResourceKey name, ResourceType type)
{  if(resources.find(name) == resources.end())
   {  return load(name, type);
   }

   return resources[name];
}

Music* ResourceLoader::getMusic(ResourceKey name)
{  return static_cast<Music*>(getResource(name, MUSIC));
}

Sound* ResourceLoader::getSound(ResourceKey name)
{  return static_cast<Sound*>(getResource(name, SOUND));
}

Tileset* ResourceLoader::getTileset(ResourceKey name)
{  return static_cast<Tileset*>(getResource(name, TILESET));
}

Region* ResourceLoader::getRegion(ResourceKey name)
{  return static_cast<Region*>(getResource(name, REGION));
}

Spritesheet* ResourceLoader::getSpritesheet(ResourceKey name)
{  return static_cast<Spritesheet*>(getResource(name, SPRITESHEET));
}

void ResourceLoader::freeAll()
{  for(std::map<ResourceKey, Resource*>::iterator i = resources.begin(); i != resources.end(); ++i)
   {  delete (i->second);
   }
}
