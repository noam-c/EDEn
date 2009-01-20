#include "ResourceLoader.h"

#include "AudioResource.h"
#include "GraphicsUtil.h"
#include "Tileset.h"
#include "Region.h"

#include "DebugUtils.h"
#include <fstream>

const int debugFlag = DEBUG_RES_LOAD;

const char* ResourceLoader::PATHS[] = {"data/music/", "data/sounds/", "data/tilesets/", "data/regions/"};
const char* ResourceLoader::EXTENSIONS[] = {".mp3", ".wav", ".edt", ".edr"};

extern std::map<ResourceLoader::Key, Resource*> ResourceLoader::resources;

char* ResourceLoader::getRegionFolder(Key name)
{  const char* regionPath = PATHS[REGION];
   const int len = strlen(regionPath) + strlen(name.c_str()) * 2 + 1;
   char* path = new char[len];

   strcpy(path, regionPath);
   strcat(path, name.c_str());
   return strcat(path, "/");
}

char* ResourceLoader::getRegionPath(Key name)
{  const char* regionFolder = getRegionFolder(name);
   const char* regionExtension = EXTENSIONS[REGION];
   const int len = strlen(regionFolder) + strlen(name.c_str()) + strlen(regionExtension);
   char* path = new char[len];

   strcpy(path, regionFolder);
   strcat(path, name.c_str());
   return strcat(path, regionExtension);
}

char* ResourceLoader::getPath(Key name, ResourceType type)
{  if( type == REGION )
   {  return getRegionPath(name);
   }
   else
   {  const int len = strlen(PATHS[type]) + strlen(name.c_str()) + strlen(EXTENSIONS[type]);
      char* path = new char[len];

      strcpy(path, PATHS[type]);
      strcat(path, name.c_str());
      return strcat(path, EXTENSIONS[type]);
   }
}

void ResourceLoader::load(Key name, ResourceType type)
{  char* path = getPath(name, type);

   try
   {  switch(type)
      {  case MUSIC:
         {  resources[name] = new Music(path);
            break;
         }
         case SOUND:
         {  resources[name] = new Sound(path);
            break;
         }
         case TILESET:
         {  resources[name] = new Tileset(path);
            break;
         }
         case REGION:
         {  resources[name] = new Region(path);
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

Mix_Music* ResourceLoader::getMusic(Key name) throw(ResourceException)
{   return static_cast<Music*>(getResource(name, MUSIC))->getMusic();
}

Mix_Chunk* ResourceLoader::getSound(Key name) throw(ResourceException)
{   return static_cast<Sound*>(getResource(name, SOUND))->getSound();
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
