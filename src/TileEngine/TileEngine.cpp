/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "TileEngine.h"

#include "stdlib.h"
#include <Rocket/Core.h>
#include <algorithm>
#include <SDL.h>

#include "ScriptEngine.h"
#include "NPC.h"
#include "PlayerCharacter.h"
#include "PlayerData.h"
#include "Scheduler.h"
#include "GraphicsUtil.h"
#include "ResourceLoader.h"
#include "Region.h"
#include "Map.h"
#include "MapExit.h"
#include "MapExitMessage.h"
#include "Pathfinder.h"
#include "Rectangle.h"
#include "DialogueController.h"
#include "GameContext.h"
#include "ExecutionStack.h"
#include "RandomTransitionGenerator.h"
#include "HomeMenu.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const int TileEngine::TILE_SIZE = 32;

TileEngine::TileEngine(GameContext& gameContext, const std::string& chapterName, const std::string& playerDataPath) :
   GameState(gameContext, GameState::FIELD, "TileEngine"),
   consoleWindow(*rocketContext),
   entityGrid(*this, messagePipe),
   playerData(gameContext),
   shortcutBar(gameContext, playerData, *rocketContext),
   mapOffset(0, 0)
{
   scheduler = new Scheduler();

   messagePipe.registerListener(this);

   loadPlayerData(playerDataPath);
   playerActor = new PlayerCharacter(messagePipe, entityGrid, playerData);
   gameContext.getScriptEngine().setTileEngine(this);
   gameContext.getScriptEngine().setPlayerData(&playerData);
   dialogue = new DialogueController(*rocketContext, *scheduler, gameContext.getScriptEngine());

   startChapter(chapterName);
}

TileEngine::~TileEngine()
{
   clearNPCs();
   messagePipe.unregisterListener(this);
   delete dialogue;
   delete playerActor;
   delete scheduler;
}

void TileEngine::loadPlayerData(const std::string& path)
{
   if(!path.empty())
   {
      playerData.load(path);
      shortcutBar.refresh();
   }
}

void TileEngine::startChapter(const std::string& chapterName)
{
   gameContext.getScriptEngine().runChapterScript(chapterName, *scheduler);
}

void TileEngine::clearNPCs()
{
   std::map<std::string, NPC*>::iterator iter;

   for(iter = npcList.begin(); iter != npcList.end(); ++iter)
   {
      delete iter->second;
   }

   npcList.clear();
}

Scheduler* TileEngine::getScheduler() const
{
   return scheduler;
}

std::string TileEngine::getMapName() const
{
   return entityGrid.getMapName();
}

void TileEngine::receive(const MapExitMessage& message)
{
   if(playerActor->isActive())
   {
      std::string exitedMap = entityGrid.getMapName();
      std::string enteredMap = message.mapExit.getNextMap();
      DEBUG("Exit signal received: Exiting %s and entering %s", exitedMap.c_str(), enteredMap.c_str());
      setMap(enteredMap);
      const shapes::Point2D& entryPoint = entityGrid.getMapData()->getMapEntrance(exitedMap);
      playerActor->addToMap(entryPoint);
   }
}

void TileEngine::dialogueNarrate(const std::string& narration, Task* task)
{
   dialogue->narrate(narration, task);
}

void TileEngine::dialogueSay(const std::string& speech, Task* task)
{
   dialogue->say(speech, task);
}

int TileEngine::setRegion(const std::string& regionName, const std::string& mapName)
{
   DEBUG("Loading region: %s", regionName.c_str());
   currRegion = ResourceLoader::getRegion(regionName);
   DEBUG("Loaded region: %s", currRegion->getName().c_str());

   return setMap(mapName);
}

int TileEngine::setMap(std::string mapName)
{
   clearNPCs();
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
      mapName = entityGrid.getMapName();
   }

   DEBUG("Map set to: %s", mapName.c_str());

   recalculateMapOffsets();

   return gameContext.getScriptEngine().runMapScript(currRegion->getName(), mapName, *scheduler);
}

void TileEngine::recalculateMapOffsets()
{
   const shapes::Size& mapPixelBounds = entityGrid.getMapBounds().getSize() * TILE_SIZE;

   mapOffset.x = mapPixelBounds.width < GraphicsUtil::getInstance()->getWidth() ?
              (GraphicsUtil::getInstance()->getWidth() - mapPixelBounds.width) >> 1 : 0;

   int totalUsableHeight = GraphicsUtil::getInstance()->getHeight() - shortcutBar.getHeight();
   mapOffset.y = mapPixelBounds.height < totalUsableHeight ?
              (totalUsableHeight - mapPixelBounds.height) >> 1 : 0;
}

void TileEngine::toggleDebugConsole()
{
   bool consoleWindowVisible = consoleWindow.isVisible();
   if(consoleWindowVisible)
   {
      consoleWindow.hide();
   }
   else
   {
      consoleWindow.show();
   }
}

NPC* TileEngine::addNPC(const std::string& npcName, const std::string& spritesheetName, const shapes::Point2D& npcLocation, const shapes::Size& size, const MovementDirection direction)
{
   NPC* npcToAdd = NULL;
   
   if(entityGrid.isAreaFree(shapes::Rectangle(npcLocation, size)))
   {
      npcToAdd = new NPC(gameContext.getScriptEngine(), *scheduler, npcName, direction,
                                 spritesheetName, messagePipe, entityGrid,
                                 currRegion->getName(), npcLocation, size);
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

void TileEngine::activate()
{
   GameState::activate();
   shortcutBar.refresh();
   playerData.bindMessagePipe(&messagePipe);
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

std::vector<Actor*> TileEngine::collectActors() const
{
   std::vector<Actor*> actors;
   std::map<std::string, NPC*>::const_iterator iter;

   for(iter = npcList.begin(); iter != npcList.end(); ++iter)
   {
      NPC* currNPC = iter->second;
      actors.push_back(currNPC);
   }

   if(playerActor->isActive())
   {
      actors.push_back(playerActor);
   }

   return actors;
}

static bool higherOnMap(const Actor* lhs, const Actor* rhs)
{
   return lhs->getLocation().y + lhs->getSize().height < rhs->getLocation().y + rhs->getSize().height;
}

void TileEngine::draw()
{
   // Collect the drawable actors and sort them by their y-location (in tiles)
   std::vector<Actor*> actors = collectActors();

   GraphicsUtil::getInstance()->clearBuffer();
   GraphicsUtil::getInstance()->setOffset(mapOffset.x, mapOffset.y);
      // Draw the map layers and actors against an offset (to center all the map elements)

      if(entityGrid.getMapData() == NULL)
      {
         // Draw all the sprites
         std::vector<Actor*>::iterator nextActorToDraw;
         for(nextActorToDraw = actors.begin(); nextActorToDraw != actors.end(); ++nextActorToDraw)
         {
            (*nextActorToDraw)->draw();
         }
      }
      else
      {
         std::sort(actors.begin(), actors.end(), higherOnMap);

         std::vector<Actor*>::iterator nextActorToDraw = actors.begin();

         const unsigned int mapHeight = entityGrid.getMapBounds().getHeight();
         for(int row = 0; row < mapHeight; ++row)
         {
            // Start by drawing a row of the background layers, if the map exists
            if(entityGrid.getMapData() != NULL)
            {
               entityGrid.drawBackground(row);
            }
         }

         for(int row = 0; row < mapHeight; ++row)
         {
            // Draw all the sprites on the row
            for(; nextActorToDraw != actors.end(); ++nextActorToDraw)
            {
               int nextActorTile = (*nextActorToDraw)->getLocation().y / TILE_SIZE;
               if(nextActorTile > row) break;
               (*nextActorToDraw)->draw();
            }

            // Draw a row of the foreground layers, if the map exists
            if(entityGrid.getMapData() != NULL)
            {
               entityGrid.drawForeground(row);
            }
         }
      }
   GraphicsUtil::getInstance()->resetOffset();
}

bool TileEngine::step(long timePassed)
{
   bool done = false;
   scheduler->runCoroutines(timePassed);

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
                  gameContext.getScriptEngine().runScriptString(*script, *scheduler);

                  // This assumes that, once the debug event is consumed here, it is not used anymore
                  delete script;
                  break;
               }
            }
            break;
         }
         case SDL_KEYDOWN:
         {
            switch(event.key.keysym.sym)
            {
               case SDLK_SPACE:
               {
                  if(!consoleWindow.isVisible())
                  {
                     if(dialogue->hasDialogue())
                     {
                        dialogue->setFastModeEnabled(true);
                        dialogue->nextLine();
                     }
                     else
                     {
                        action();
                     }
                  }
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
               case SDLK_TAB:
               {
                  if(!consoleWindow.isVisible())
                  {
                     // Clear the message pipe in preparation for the new state.
                     // The Tile Engine message pipe will be rebound when the Tile Engine state
                     // is activated again.
                     playerData.clearMessagePipe();

                     HomeMenu* menu = new HomeMenu(gameContext, playerData);
                     gameContext.getExecutionStack().pushState(menu, RandomTransitionGenerator::create(gameContext, this, menu));
                     return;
                  }
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
            if(!consoleWindow.isVisible())
            {
               switch(event.key.keysym.sym)
               {
                  case SDLK_SPACE:
                  {
                     if(dialogue->hasDialogue())
                     {
                        dialogue->setFastModeEnabled(false);
                     }
                     return;
                  }
                  default:
                  {
                     break;
                  }
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

      // If the tile engine didn't consume this event, then propagate to the generic input handling
      handleEvent(event);
   }
}

void TileEngine::action()
{
   NPC* npcToActivate = static_cast<NPC*>(entityGrid.getAdjacentActor(playerActor));
   if(npcToActivate != NULL)
   {
      npcToActivate->activate();
   }
}
