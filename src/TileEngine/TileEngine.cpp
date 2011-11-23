#include "TileEngine.h"
#include "ScriptEngine.h"
#include "NPC.h"
#include "PlayerCharacter.h"
#include "Scheduler.h"
#include "Container.h"
#include "GraphicsUtil.h"
#include "ResourceLoader.h"
#include "Region.h"
#include "Map.h"
#include "DebugConsoleWindow.h"
#include "DialogueController.h"
#include "OpenGLTTF.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const int TileEngine::TILE_SIZE = 32;

TileEngine::TileEngine(ExecutionStack& executionStack, const std::string& chapterName)
                                   : GameState(executionStack), currMap(NULL), xMapOffset(0), yMapOffset(0)
{
   player = new PlayerCharacter(ResourceLoader::getSpritesheet("npc1"), 320, 320);
   scriptEngine = new ScriptEngine(*this, scheduler);
   dialogue = new DialogueController(*top, scheduler, *scriptEngine);

   consoleWindow = new edwt::DebugConsoleWindow(top, top->getWidth(), top->getHeight() * 0.2);

   time = SDL_GetTicks();
   scriptEngine->runChapterScript(chapterName);
}

std::string TileEngine::getMapName()
{
   return currMap->getName();
}

void TileEngine::dialogueNarrate(const char* narration, Task* task)
{
   dialogue->narrate(narration, task);
}

void TileEngine::dialogueSay(const char* speech, Task* task)
{
   dialogue->say(speech, task);
}

bool TileEngine::setRegion(const std::string& regionName, const std::string& mapName)
{
   DEBUG("Loading region: %s", regionName.c_str());
   currRegion = ResourceLoader::getRegion(regionName);
   DEBUG("Loaded region: %s", currRegion->getName().c_str());

   setMap(mapName);

   DEBUG("Running map script: %s/%s", regionName.c_str(), currMap->getName().c_str());
   return true;
}

void TileEngine::setMap(std::string mapName)
{
   DEBUG("Setting map...");
   if(!mapName.empty())
   {
      // If a map name was supplied, then we set this map and run its script
      currMap = currRegion->getMap(mapName);
   }
   else
   {
      // Otherwise, we use the region's default starting map
      currMap = currRegion->getStartingMap();
      mapName = currMap->getName();
   }

   DEBUG("Map set to: %s", mapName.c_str());

   recalculateMapOffsets();
}

void TileEngine::recalculateMapOffsets()
{
   const int mapPixelWidth = currMap->getWidth() * TILE_SIZE;
   const int mapPixelHeight = currMap->getHeight() * TILE_SIZE;

   xMapOffset = mapPixelWidth < GraphicsUtil::width ? 
              (GraphicsUtil::width - mapPixelWidth) >> 1 : 0;

   yMapOffset = mapPixelHeight < GraphicsUtil::height ?
              (GraphicsUtil::height - mapPixelHeight) >> 1 : 0;
}

void TileEngine::toggleDebugConsole()
{
   bool consoleWindowVisible = consoleWindow->isVisible();
   if(consoleWindowVisible)
   {
      // Hide the console window
      consoleWindow->setVisible(false);
   }
   else
   {
      // Show the console window
      top->moveToTop(consoleWindow);
      consoleWindow->setVisible(true);
      consoleWindow->requestFocus();
   }
}

void TileEngine::addNPC(const std::string& npcName, const std::string& spritesheetName, int x, int y)
{
   Spritesheet* sheet = ResourceLoader::getSpritesheet(spritesheetName);
   npcList[npcName] = new NPC(*scriptEngine, scheduler, npcName, sheet,
                              *currMap, currRegion->getName(),
                              x * TILE_SIZE, y * TILE_SIZE);
}

bool TileEngine::withinMap(int x, int y)
{
   return x >= 0 && x < currMap->getWidth() * TILE_SIZE && y >= 0 && y < currMap->getHeight() * TILE_SIZE;
}

void TileEngine::moveNPC(const std::string& npcName, int x, int y)
{
   NPC* npcToMove = npcList[npcName];
   if(withinMap(x,y))
   {
      DEBUG("Sending move order to %s: %d,%d", npcName.c_str(), x, y);
      npcToMove->move(x, y);
   }
   else
   {
      DEBUG("%d,%d is not a place!!!", x, y);
   }
}

void TileEngine::setNPCSprite(const std::string& npcName, const std::string& frameName)
{
   NPC* npcToChange = npcList[npcName];
   npcToChange->setFrame(frameName);
}

void TileEngine::setNPCAnimation(const std::string& npcName, const std::string& animationName)
{
   NPC* npcToChange = npcList[npcName];
   npcToChange->setAnimation(animationName);
}

void TileEngine::changeNPCSpritesheet(const std::string& npcName, const std::string& spritesheetName)
{
   Spritesheet* sheet = ResourceLoader::getSpritesheet(spritesheetName);
   NPC* npcToChange = npcList[npcName];
   npcToChange->setSpritesheet(sheet);
}

void TileEngine::stepNPCs(long timePassed)
{
   std::map<std::string, NPC*>::iterator iter;

   for(iter = npcList.begin(); iter != npcList.end(); ++iter)
   {
      NPC* currNPC = iter->second;
      currNPC->step(timePassed);
   }
}

void TileEngine::drawNPCs()
{
   std::map<std::string, NPC*>::iterator iter;

   for(iter = npcList.begin(); iter != npcList.end(); ++iter)
   {
      NPC* currNPC = iter->second;
      currNPC->draw();
   }
}

void TileEngine::draw()
{
   GraphicsUtil::getInstance()->setOffset(xMapOffset, yMapOffset);
      // Draw the map and NPCs against an offset (to center all the map elements)
      if(currMap != NULL)
      {
         currMap->draw();
      }
      else
      {
         GraphicsUtil::getInstance()->clearBuffer();
      }

      drawNPCs();
      player->draw();
   GraphicsUtil::getInstance()->resetOffset();
}

bool TileEngine::step()
{
   long prevTime = time;
   time = SDL_GetTicks();

   long timePassed = time - prevTime;
   if(timePassed > 32)
   {
      timePassed = 32;
   }

   bool done = false;
   scheduler.runThreads(timePassed);

   handleInputEvents(done);

   player->step(timePassed);

   if(currMap != NULL)
   {
      currMap->step(timePassed);
   }

   stepNPCs(timePassed);

   return !done;
}

void TileEngine::handleInputEvents(bool& finishState)
{
   SDL_Event event;

   while(SDL_PollEvent(&event))
   {
      switch (event.type)
      {
         case SDL_USEREVENT:
         {
            switch(event.user.code)
            {
               case DEBUG_CONSOLE_EVENT:
               {
                  std::string* script = (std::string*)event.user.data1;
                  scriptEngine->runScriptString(*script);

                  // This assumes that, once the debug event is consumed here, it is not used anymore
                  delete script;
               }
            }
         }
         case SDL_KEYDOWN:
         {
            switch(event.key.keysym.sym)
            {
               case SDLK_SPACE:
               {
                  dialogue->setFastModeEnabled(true);
                  dialogue->nextLine();
                  return;
               }
               case SDLK_ESCAPE:
               {
                  finishState = true;
                  return;
               }
               case SDLK_BACKQUOTE:
               {
                  toggleDebugConsole();
                  return;
               }
               default:
               {
                  break;
               }
            }
            break;
         }
         case SDL_KEYUP:
         {
            switch(event.key.keysym.sym)
            {
               case SDLK_SPACE:
               {
                  dialogue->setFastModeEnabled(false);
                  return;
               }
               default:
               {
                  break;
               }
            }
            break;
         }
         case SDL_QUIT:
         {
            finishState = true;
            return;
         }
         default:
         {
            break;
         }
      }
   }

   // If the tile engine didn't consume this event, then propagate to the generic input handling
   handleEvent(event);
}

TileEngine::~TileEngine()
{
   delete consoleWindow;
   delete dialogue;
   delete scriptEngine;
   delete player;
}
