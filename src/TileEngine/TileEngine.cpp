#include "TileEngine.h"
#include "Region.h"
#include "ScriptEngine.h"
#include "Container.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_TILE_ENG;

TileEngine::TileEngine() : currMap(NULL)
{   dialogue = new DialogueController(top);
    time = SDL_GetTicks();
}

void TileEngine::dialogueNarrate(const char* narration, TicketId ticket)
{   dialogue->narrate(narration, ticket);
}

void TileEngine::dialogueSay(const char* speech, TicketId ticket)
{   dialogue->say(speech, ticket);
}

std::string TileEngine::setRegion(std::string regionName, std::string mapName)
{  try
   {  DEBUG("Loading region: %s", regionName.c_str());
      currRegion = ResourceLoader::getRegion(regionName);
      DEBUG("Loaded region: %s", currRegion->getName().c_str());

      DEBUG("Setting map...");
      if(!mapName.empty())
      {  // If a map name was supplied, then we set this map and run its script
         currMap = currRegion->getMap(mapName);
      }
      else
      {  // Otherwise, we use the region's default starting map
         currMap = currRegion->getStartingMap();
         mapName = currMap->getName();
      }

      DEBUG("Map set to: %s", mapName.c_str());

      std::string scriptFolder(ResourceLoader::getRegionFolder(regionName));
      DEBUG("Running map script: %s%s", scriptFolder.c_str(), mapName.c_str());
      return (scriptFolder + mapName);
   }
   catch(ResourceException e)
   {  DEBUG(e.what());
   }
}

void TileEngine::draw()
{  if(currMap != NULL)
   {   currMap->draw();
   }
   else
   {   GraphicsUtil::getInstance()->clearBuffer();
   }

   GameState::draw();
   GraphicsUtil::getInstance()->flipScreen();
}

bool TileEngine::step()
{  long timePassed = SDL_GetTicks() - time;
   time += timePassed;
   GameState::step();

   bool done = false;
   ScriptEngine::getInstance()->runThreads(timePassed);
   dialogue->timePassed(timePassed);
   SDL_Event event;

   /* Check for events */
   while(SDL_PollEvent(&event))
   {  switch (event.type)
      {   case SDL_USEREVENT:
          {  case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {  case SDLK_SPACE:
                   {  dialogue->nextLine();
                      break;
                   }
                   case SDLK_ESCAPE:
                   {  done = true;
                      break;
                   }
                }
                break;
             case SDL_QUIT:
                done = true;
                break;
             default:
                break;
          }
      }

      GraphicsUtil::getInstance()->pushInput(event);
   }

   return !done;
}

TileEngine::~TileEngine()
{  delete dialogue;
   delete top;
}
