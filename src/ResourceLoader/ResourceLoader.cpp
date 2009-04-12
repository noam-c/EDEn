#include "ResourceLoader.h"

#include "Music.h"
#include "Sound.h"
#include "GraphicsUtil.h"
#include "Tileset.h"
#include "Region.h"

#include "DebugUtils.h"
#include <fstream>

const int debugFlag = DEBUG_RES_LOAD;

const std::string ResourceLoader::PATHS[] = {"data/music/", "data/sounds/", "data/tilesets/", "data/regions/"};
const std::string ResourceLoader::EXTENSIONS[] = {".mp3", ".wav", ".edt", ".edr"};

extern std::map<ResourceLoader::Key, Resource*> ResourceLoader::resources;

std::string ResourceLoader::getRegionFolder(Key name)
{  return PATHS[REGION] + name + '/';
}

std::string ResourceLoader::getRegionPath(Key name)
{  return getRegionFolder(name) + name + EXTENSIONS[REGION];
}

std::string ResourceLoader::getPath(Key name, ResourceType type)
{  if( type == REGION )
   {  return getRegionPath(name);
   }
   else
   {  return PATHS[type] + name + EXTENSIONS[type];
   }
}

void ResourceLoader::load(Key name, ResourceType type)
{  std::string path = getPath(name, type);

   try
   {  switch(type)
      {  case MUSIC:
         {  resources[name] = new Music(path.c_str());
            break;
         }
         case SOUND:
         {  resources[name] = new Sound(path.c_str());
            break;
         }
         case TILESET:
         {  resources[name] = new Tileset(path.c_str());
            break;
         }
         case REGION:
         {  resources[name] = new Region(path.c_str());
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

Resource* ResourceLoader::getResource(Key name, ResourceType type) throw(ResourceException)
{   if(resources.find(name) == resources.end())
    {   load(name, type);
    }

    return resources[name];
}

Music* ResourceLoader::getMusic(Key name) throw(ResourceException)
{   return static_cast<Music*>(getResource(name, MUSIC));
}

Sound* ResourceLoader::getSound(Key name) throw(ResourceException)
{  return static_cast<Sound*>(getResource(name, SOUND));
}

Tileset* ResourceLoader::getTileset(Key name) throw(ResourceException)
{   return static_cast<Tileset*>(getResource(name, TILESET));
}

Region* ResourceLoader::getRegion(Key name) throw(ResourceException)
{   return static_cast<Region*>(getResource(name, REGION));
}

void ResourceLoader::freeAll()
{   for(std::map<ResourceLoader::Key, Resource*>::iterator i = resources.begin(); i != resources.end(); ++i)
    {  delete (i->second);
    }
}
