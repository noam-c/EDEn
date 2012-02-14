/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "TileEngine.h"
#include "ScriptEngine.h"
#include "NPC.h"
#include "PlayerCharacter.h"
#include "PlayerData.h"
#include "Scheduler.h"
#include "Container.h"
#include "GraphicsUtil.h"
#include "ResourceLoader.h"
#include "Region.h"
#include "Map.h"
#include "Pathfinder.h"
#include "DebugConsoleWindow.h"
#include "DialogueController.h"
#include "OpenGLTTF.h"
#include "stdlib.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const int TileEngine::TILE_SIZE = 32;

TileEngine::TileEngine(ExecutionStack& executionStack, const std::string& chapterName, const std::string& playerDataPath)
: GameState(executionStack), xMapOffset(0), yMapOffset(0)
{
   playerEntity = new PlayerCharacter(currMap, ResourceLoader::getSpritesheet("npc1"));
   scriptEngine = new ScriptEngine(*this, playerData, scheduler);
   dialogue = new DialogueController(*top, scheduler, *scriptEngine);
   consoleWindow = new edwt::DebugConsoleWindow(top, top->getWidth(), top->getHeight() * 0.2);
   
   time = SDL_GetTicks();
   loadPlayerData(playerDataPath);
   startChapter(chapterName);
}

TileEngine::~TileEngine()
{
   delete consoleWindow;
   delete dialogue;
   delete scriptEngine;
   delete playerEntity;
}

void TileEngine::loadPlayerData(const std::string& path)
{
   if(!path.empty())
   {
      playerData.load(path);
   }
}

void TileEngine::startChapter(const std::string& chapterName)
{
   scriptEngine->runChapterScript(chapterName);
}

std::string TileEngine::getMapName()
{
   return currMap.getName();
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

   DEBUG("Running map script: %s/%s", regionName.c_str(), currMap.getName().c_str());
   return true;
}

void TileEngine::setMap(std::string mapName)
{
   playerEntity->removeFromMap();

   DEBUG("Setting map...");
   if(!mapName.empty())
   {
      // If a map name was supplied, then we set this map and run its script
      currMap.setMapData(currRegion->getMap(mapName));
   }
   else
   {
      // Otherwise, we use the region's default starting map
      currMap.setMapData(currRegion->getStartingMap());
      mapName = currMap.getName();
   }

   DEBUG("Map set to: %s", mapName.c_str());

   recalculateMapOffsets();
}

void TileEngine::recalculateMapOffsets()
{
   const int mapPixelWidth = currMap.getWidth() * TILE_SIZE;
   const int mapPixelHeight = currMap.getHeight() * TILE_SIZE;

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

void TileEngine::showPlayer(int x, int y)
{
   playerEntity->addToMap(Point2D(x,y));
}

void TileEngine::hidePlayer()
{
   playerEntity->removeFromMap();   
}

void TileEngine::setPlayerLocation(int x, int y)
{
   playerEntity->setLocation(Point2D(x,y));
}

void TileEngine::addNPC(const std::string& npcName, const std::string& spritesheetName, int x, int y)
{
   Point2D npcLocation(x, y);
   if(currMap.isAreaFree(npcLocation, 32, 32))
   {
      Spritesheet* sheet = ResourceLoader::getSpritesheet(spritesheetName);
      NPC* npcToAdd = new NPC(*scriptEngine, scheduler, npcName, sheet,
                                 currMap, currRegion->getName(),
                                 npcLocation.x, npcLocation.y);
      npcList[npcName] = npcToAdd;
      currMap.addNPC(npcToAdd, npcLocation, 32, 32);
   }
   else
   {
      DEBUG("Cannot place NPC at this location; something is in the way.");
   }
}

bool TileEngine::withinMap(Point2D point) const
{
   return withinMap(point.x, point.y);
}


bool TileEngine::withinMap(const int x, const int y) const
{
   return x >= 0 && x < currMap.getWidth() * TILE_SIZE && y >= 0 && y < currMap.getHeight() * TILE_SIZE;
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

void TileEngine::turnNPCTowardsPlayer(const std::string& npcName)
{
   NPC* npcToChange = npcList[npcName];
   Point2D playerLocation = playerEntity->getLocation();
   Point2D npcLocation = npcToChange->getLocation();
   MovementDirection directionToPlayer = npcToChange->getDirection();
   
   int xDiff = npcLocation.x - playerLocation.x;
   int yDiff = npcLocation.y - playerLocation.y;
   
   if(abs(xDiff) > abs(yDiff))
   {
      if(xDiff < 0)
      {
         directionToPlayer = RIGHT;
      }
      else if(xDiff > 0)
      {
         directionToPlayer = LEFT;
      }  
   }
   else
   {
      if(yDiff < 0)
      {
         directionToPlayer = DOWN;
      }
      else if(yDiff > 0)
      {
         directionToPlayer = UP;
      }
   }

   npcToChange->stand(directionToPlayer);
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
      if(currMap.getMapData() != NULL)
      {
         currMap.draw();
      }
      else
      {
         GraphicsUtil::getInstance()->clearBuffer();
      }

      drawNPCs();
      playerEntity->draw();
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

   playerEntity->step(timePassed);

   currMap.step(timePassed);

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
                  if(dialogue->hasDialogue())
                  {
                     dialogue->setFastModeEnabled(false);
                  }
                  else
                  {
                     action();
                  }
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

void TileEngine::action()
{
   Point2D activationLocation = playerEntity->getLocation();
   switch(playerEntity->getDirection())
   {
      case UP_LEFT:
      case LEFT:
      case DOWN_LEFT:
      {
         activationLocation.x -= TILE_SIZE;
         break;
      }
      case UP_RIGHT:
      case RIGHT:
      case DOWN_RIGHT:
      {
         activationLocation.x += TILE_SIZE;
         break;
      }
      default:
      {
         break;
      }   
   }

   switch(playerEntity->getDirection())
   {
      case UP_RIGHT:
      case UP:
      case UP_LEFT:
      {
         activationLocation.y -= TILE_SIZE;
         break;
      }
      case DOWN_LEFT:
      case DOWN:
      case DOWN_RIGHT:
      {
         activationLocation.y += TILE_SIZE;
         break;
      }
      default:
      {
         break;
      }
   }
   
   if(withinMap(activationLocation))
   {
      NPC* npcToActivate = currMap.getOccupantNPC(activationLocation, TILE_SIZE, TILE_SIZE);
      if(npcToActivate != NULL)
      {
         npcToActivate->activate();
      }
   }
}
