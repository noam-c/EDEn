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

void ResourceLoader::load(ResourceKey name, ResourceType type)
{  std::string path = getPath(name, type);

   try
   {  switch(type)
      {  case MUSIC:
         {  resources[name] = new Music(name, path.c_str());
            break;
         }
         case SOUND:
         {  resources[name] = new Sound(name, path.c_str());
            break;
         }
         case TILESET:
         {  resources[name] = new Tileset(name, path.c_str());
            break;
         }
         case REGION:
         {  resources[name] = new Region(name, path.c_str());
            break;
         }
         case SPRITESHEET:
         {  resources[name] = new Spritesheet(name, path.c_str());
            break;
         }
      }
   }
   catch(Exception e)
   {  delete resources[name];
      resources[name] = NULL;
      throw ResourceException(e, name);
   }
}

Resource* ResourceLoader::getResource(ResourceKey name, ResourceType type) throw(ResourceException)
{   if(resources.find(name) == resources.end())
    {   load(name, type);
    }

    return resources[name];
}

Music* ResourceLoader::getMusic(ResourceKey name) throw(ResourceException)
{   return static_cast<Music*>(getResource(name, MUSIC));
}

Sound* ResourceLoader::getSound(ResourceKey name) throw(ResourceException)
{  return static_cast<Sound*>(getResource(name, SOUND));
}

Tileset* ResourceLoader::getTileset(ResourceKey name) throw(ResourceException)
{   return static_cast<Tileset*>(getResource(name, TILESET));
}

Region* ResourceLoader::getRegion(ResourceKey name) throw(ResourceException)
{   return static_cast<Region*>(getResource(name, REGION));
}

Spritesheet* ResourceLoader::getSpritesheet(ResourceKey name) throw(ResourceException)
{   return static_cast<Spritesheet*>(getResource(name, SPRITESHEET));
}

void ResourceLoader::freeAll()
{   for(std::map<ResourceKey, Resource*>::iterator i = resources.begin(); i != resources.end(); ++i)
    {  delete (i->second);
    }
}
