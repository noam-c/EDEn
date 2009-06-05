#include "TileEngine.h"
#include "Region.h"
#include "ScriptEngine.h"
#include "NPC.h"
#include "Scheduler.h"
#include "Container.h"
#include "ScriptFactory.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TILE_ENG;

TileEngine::TileEngine(std::string chapterName) : currMap(NULL)
{   scheduler = new Scheduler();
    scriptEngine = new ScriptEngine(this, scheduler);
    dialogue = new DialogueController(top, scriptEngine);
    time = SDL_GetTicks();
    scheduler->start(dialogue);
    Script* introScript = ScriptFactory::getChapterScript(scriptEngine->getVM(), chapterName);
    scriptEngine->runScript(introScript);
}

std::string TileEngine::getMapName()
{   return currMap->getName();
}

void TileEngine::dialogueNarrate(const char* narration, Task* task)
{   dialogue->narrate(narration, task);
}

void TileEngine::dialogueSay(const char* speech, Task* task)
{   dialogue->say(speech, task);
}

bool TileEngine::setRegion(std::string regionName, std::string mapName)
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

      DEBUG("Running map script: %s/%s", regionName.c_str(), mapName.c_str());
      return true;
   }
   catch(ResourceException e)
   {  DEBUG(e.what());
   }

   return false;
}

void TileEngine::addNPC(NPC* npc)
{  
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
   scheduler->runThreads(timePassed);
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
{  delete scheduler;
   delete scriptEngine;
   delete dialogue;
   delete top;
}
