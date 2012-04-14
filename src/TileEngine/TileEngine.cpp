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
#include "Rectangle.h"
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
   playerActor = new PlayerCharacter(entityGrid, "npc1");
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
   delete playerActor;
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
   return entityGrid.getName();
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

   DEBUG("Running map script: %s/%s", regionName.c_str(), entityGrid.getName().c_str());
   return true;
}

void TileEngine::setMap(std::string mapName)
{
   playerActor->removeFromMap();

   DEBUG("Setting map...");
   if(!mapName.empty())
   {
      // If a map name was supplied, then we set this map and run its script
      entityGrid.setMapData(currRegion->getMap(mapName));
   }
   else
   {
      // Otherwise, we use the region's default starting map
      entityGrid.setMapData(currRegion->getStartingMap());
      mapName = entityGrid.getName();
   }

   DEBUG("Map set to: %s", mapName.c_str());

   recalculateMapOffsets();
}

void TileEngine::recalculateMapOffsets()
{
   const shapes::Size& mapPixelBounds = entityGrid.getBounds().getSize() * TILE_SIZE;

   xMapOffset = mapPixelBounds.width < GraphicsUtil::width ? 
              (GraphicsUtil::width - mapPixelBounds.width) >> 1 : 0;

   yMapOffset = mapPixelBounds.height < GraphicsUtil::height ?
              (GraphicsUtil::height - mapPixelBounds.height) >> 1 : 0;
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

NPC* TileEngine::addNPC(const std::string& npcName, const std::string& spritesheetName, const shapes::Point2D& npcLocation, const shapes::Size& size)
{
   NPC* npcToAdd = NULL;
   
   if(entityGrid.isAreaFree(shapes::Rectangle(npcLocation, size)))
   {
      npcToAdd = new NPC(*scriptEngine, scheduler, npcName, spritesheetName,
                                 entityGrid, currRegion->getName(),
                                 npcLocation, size);
      npcList[npcName] = npcToAdd;
      entityGrid.addActor(npcToAdd, npcLocation);
   }
   else
   {
      DEBUG("Cannot place NPC at this location; something is in the way.");
   }

   return npcToAdd;
}

NPC* TileEngine::getNPC(const std::string& npcName) const
{
   std::map<std::string, NPC*>::const_iterator npcIterator = npcList.find(npcName);
   if(npcIterator != npcList.end())
   {
      return npcIterator->second;
   }

   return NULL;
}

PlayerCharacter* TileEngine::getPlayerCharacter() const
{
   return playerActor;
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
      if(entityGrid.getMapData() != NULL)
      {
         entityGrid.draw();
      }
      else
      {
         GraphicsUtil::getInstance()->clearBuffer();
      }

      drawNPCs();
      playerActor->draw();
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

   playerActor->step(timePassed);

   entityGrid.step(timePassed);

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
   NPC* npcToActivate = static_cast<NPC*>(entityGrid.getAdjacentActor(playerActor));
   if(npcToActivate != NULL)
   {
      npcToActivate->activate();
   }
}
