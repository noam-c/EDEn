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
#include "MapTriggerMessage.h"
#include "TriggerZone.h"
#include "Pathfinder.h"
#include "Rectangle.h"
#include "DialogueController.h"
#include "ExecutionStack.h"
#include "CameraSlider.h"
#include "RandomTransitionGenerator.h"
#include "HomeMenu.h"
#include "MapTriggerCallback.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const int TileEngine::TILE_SIZE = 32;

TileEngine::TileEngine(GameContext& gameContext, const std::string& chapterName, const std::string& playerDataPath) :
   GameState(gameContext, GameState::FIELD, "TileEngine"),
   m_consoleWindow(*m_rocketContext),
   m_entityGrid(*this, m_messagePipe),
   m_shortcutBar(getCurrentPlayerData(), getScriptEngine(), getMetadata(), getStateType(), *m_rocketContext)
{
   m_scheduler = new Scheduler();

   m_messagePipe.registerListener<MapExitMessage>(this);
   m_messagePipe.registerListener<MapTriggerMessage>(this);

   loadPlayerData(playerDataPath);
   m_playerActor = new PlayerCharacter(m_messagePipe, m_entityGrid, getCurrentPlayerData());
   m_cameraTarget = m_playerActor;
   getScriptEngine().setTileEngine(this);
   m_dialogue = new DialogueController(*m_rocketContext, *m_scheduler, getScriptEngine());

   startChapter(chapterName);
}

TileEngine::~TileEngine()
{
   clearMapListeners();
   clearNPCs();
   m_messagePipe.unregisterListener<MapTriggerMessage>(this);
   m_messagePipe.unregisterListener<MapExitMessage>(this);
   delete m_dialogue;
   delete m_playerActor;
   delete m_scheduler;
}

void TileEngine::loadPlayerData(const std::string& path)
{
   if(!path.empty())
   {
      getCurrentPlayerData().load(path);
      m_shortcutBar.refresh();
   }
}

void TileEngine::startChapter(const std::string& chapterName)
{
   getScriptEngine().runChapterScript(chapterName, *m_scheduler);
}

void TileEngine::clearMapListeners()
{
   std::vector<std::pair<std::string, MapTriggerCallback*> >::iterator iter;

   for(iter = m_triggerScripts.begin(); iter != m_triggerScripts.end(); ++iter)
   {
      delete iter->second;
   }
   
   m_triggerScripts.clear();
}

void TileEngine::clearNPCs()
{
   std::map<std::string, NPC*>::iterator iter;

   for(iter = m_npcList.begin(); iter != m_npcList.end(); ++iter)
   {
      delete iter->second;
   }

   m_npcList.clear();
}

Scheduler* TileEngine::getScheduler() const
{
   return m_scheduler;
}

std::string TileEngine::getMapName() const
{
   return m_entityGrid.getMapName();
}

void TileEngine::receive(const MapExitMessage& message)
{
   if(m_playerActor->isActive())
   {
      m_playerActor->removeFromMap();
      std::string exitedMap = m_entityGrid.getMapName();
      std::string enteredMap = message.mapExit.getNextMap();
      DEBUG("Exit signal received: Exiting %s and entering %s", exitedMap.c_str(), enteredMap.c_str());
      setMap(enteredMap);
      const shapes::Point2D& entryPoint = m_entityGrid.getMapEntrance(exitedMap);
      m_playerActor->addToMap(entryPoint);
      followWithCamera(m_playerActor);
   }
}

void TileEngine::receive(const MapTriggerMessage& message)
{
   for(std::vector<std::pair<std::string, MapTriggerCallback*> >::iterator iter = m_triggerScripts.begin(); iter != m_triggerScripts.end(); ++iter)
   {
      if(iter->first == message.triggerZone.getName())
      {
         Actor* triggeringActor =
            message.triggeringActor == m_playerActor ?
            static_cast<Actor*>(m_playerActor) :
            getNPC(message.triggeringActor->getName());
         
         iter->second->callback(triggeringActor);
      }
   }
}

void TileEngine::dialogueNarrate(const std::string& narration, const std::shared_ptr<Task>& task)
{
   m_dialogue->narrate(narration, task);
}

void TileEngine::dialogueSay(const std::string& speech, const std::shared_ptr<Task>& task)
{
   m_dialogue->say(speech, task);
}

int TileEngine::setRegion(const std::string& regionName, const std::string& mapName)
{
   DEBUG("Loading region: %s", regionName.c_str());
   m_currRegion = ResourceLoader::getRegion(regionName);
   DEBUG("Loaded region: %s", m_currRegion->getName().c_str());

   return setMap(mapName);
}

int TileEngine::setMap(std::string mapName)
{
   clearMapListeners();
   clearNPCs();
   m_playerActor->removeFromMap();

   DEBUG("Setting map...");
   if(!mapName.empty())
   {
      // If a map name was supplied, then we set this map and run its script
      m_entityGrid.setMapData(m_currRegion->getMap(mapName));
   }
   else
   {
      // Otherwise, we use the region's default starting map
      m_entityGrid.setMapData(m_currRegion->getStartingMap());
      mapName = m_entityGrid.getMapName();
   }

   DEBUG("Map set to: %s", mapName.c_str());

   recalculateMapOffsets();
   return getScriptEngine().runMapScript(m_currRegion->getName(), mapName, *m_scheduler);
}

void TileEngine::followWithCamera(const Actor* target)
{
   m_cameraTarget = target;
}

void TileEngine::releaseCamera()
{
   m_cameraTarget = nullptr;
}

int TileEngine::slideCamera(const shapes::Point2D& origin, const shapes::Point2D& destination, double speed)
{
   if(speed > 0)
   {
      auto slider = std::make_shared<CameraSlider>(m_camera, origin, destination, speed);
      m_cameraTarget = nullptr;
      m_scheduler->start(slider);
      return m_scheduler->join(slider);
   }

   return 0;
}

shapes::Point2D TileEngine::getCurrentCameraLocation() const
{
   if(m_cameraTarget != nullptr)
   {
      return m_cameraTarget->getLocation();
   }
   else
   {
      return m_camera.getFocalPoint();
   }
}

void TileEngine::recalculateMapOffsets()
{
   const shapes::Size mapPixelBounds = m_entityGrid.hasMapData() ?
      m_entityGrid.getMapBounds().getSize() * TILE_SIZE :
      shapes::Size();

   const int totalUsableHeight = GraphicsUtil::getInstance()->getHeight() - m_shortcutBar.getHeight();
   const shapes::Size screenSize(GraphicsUtil::getInstance()->getWidth(), totalUsableHeight);
   m_camera.setViewBounds(screenSize, mapPixelBounds);
}

void TileEngine::toggleDebugConsole()
{
   bool consoleWindowVisible = m_consoleWindow.isVisible();
   if(consoleWindowVisible)
   {
      m_consoleWindow.hide();
   }
   else
   {
      m_consoleWindow.show();
   }
}

NPC* TileEngine::addNPC(const std::string& npcName, const std::string& spritesheetName, const shapes::Point2D& npcLocation, const shapes::Size& size, const MovementDirection direction)
{
   NPC* npcToAdd = nullptr;
   
   if(m_entityGrid.isAreaFree(shapes::Rectangle(npcLocation, size)))
   {
      npcToAdd = new NPC(getScriptEngine(), *m_scheduler, npcName, direction,
                                 spritesheetName, m_messagePipe, m_entityGrid,
                                 m_currRegion->getName(), npcLocation, size);
      m_npcList[npcName] = npcToAdd;
      m_entityGrid.addActor(npcToAdd, npcLocation);
   }
   else
   {
      DEBUG("Cannot place NPC at this location; something is in the way.");
   }

   return npcToAdd;
}

NPC* TileEngine::getNPC(const std::string& npcName) const
{
   std::map<std::string, NPC*>::const_iterator npcIterator = m_npcList.find(npcName);
   if(npcIterator != m_npcList.end())
   {
      return npcIterator->second;
   }

   return nullptr;
}

void TileEngine::addTriggerListener(const std::string& triggerName, MapTriggerCallback* callback)
{
   m_triggerScripts.push_back(std::pair<std::string, MapTriggerCallback*>(triggerName, callback));
}

PlayerCharacter* TileEngine::getPlayerCharacter() const
{
   return m_playerActor;
}

void TileEngine::activate()
{
   GameState::activate();
   m_shortcutBar.refresh();
   recalculateMapOffsets();
   getCurrentPlayerData().bindMessagePipe(&m_messagePipe);
}

void TileEngine::stepNPCs(long timePassed)
{
   std::map<std::string, NPC*>::iterator iter;

   for(iter = m_npcList.begin(); iter != m_npcList.end(); ++iter)
   {
      NPC* currNPC = iter->second;
      currNPC->step(timePassed);
   }
}

std::vector<Actor*> TileEngine::collectActors() const
{
   std::vector<Actor*> actors;
   std::map<std::string, NPC*>::const_iterator iter;

   for(iter = m_npcList.begin(); iter != m_npcList.end(); ++iter)
   {
      NPC* currNPC = iter->second;
      actors.push_back(currNPC);
   }

   if(m_playerActor->isActive())
   {
      actors.push_back(m_playerActor);
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

   m_camera.apply();
      if(!m_entityGrid.hasMapData())
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

         const unsigned int mapHeight = m_entityGrid.getMapBounds().getHeight();
         for(int row = 0; row < mapHeight; ++row)
         {
            // Start by drawing a row of the background layers, if the map exists
            if(m_entityGrid.hasMapData())
            {
               m_entityGrid.drawBackground(row);
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
            if(m_entityGrid.hasMapData())
            {
               m_entityGrid.drawForeground(row);
            }
         }
      }

   m_camera.reset();
}

bool TileEngine::step(long timePassed)
{
   bool done = false;
   m_scheduler->runCoroutines(timePassed);

   handleInputEvents(done);

   m_playerActor->step(timePassed);
   m_entityGrid.step(timePassed);

   stepNPCs(timePassed);

   if(m_cameraTarget != nullptr &&
      (m_cameraTarget != m_playerActor || m_playerActor->isActive()))
   {
      m_camera.setFocalPoint(m_cameraTarget->getLocation());
   }

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
                  getScriptEngine().runScriptString(*script);

                  // This assumes that, once the debug event is consumed here, it is not used anymore
                  delete script;
                  break;
               }
            }
            break;
         }
         case SDL_MOUSEBUTTONDOWN:
         {
            const shapes::Point2D mouseClickLocation(event.button.x, event.button.y);
            if(m_camera.isPointWithinViewport(mouseClickLocation))
            {
               const shapes::Point2D pointWithinScene = m_camera.getPointWithinScene(mouseClickLocation);
               
               if(m_entityGrid.isAreaFree(shapes::Rectangle(pointWithinScene, m_playerActor->getSize())))
               {
                  m_playerActor->move(pointWithinScene);
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
                  if(!m_consoleWindow.isVisible())
                  {
                     if(m_dialogue->hasDialogue())
                     {
                        m_dialogue->setFastModeEnabled(true);
                        m_dialogue->nextLine();
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
                  if(!m_consoleWindow.isVisible())
                  {
                     // Clear the message pipe in preparation for the new state.
                     // The Tile Engine message pipe will be rebound when the Tile Engine state
                     // is activated again.
                     getCurrentPlayerData().clearMessagePipe();

                     HomeMenu* menu = new HomeMenu(m_gameContext);
                     getExecutionStack()->pushState(menu, RandomTransitionGenerator::create(m_gameContext, this, menu));
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
            if(!m_consoleWindow.isVisible())
            {
               switch(event.key.keysym.sym)
               {
                  case SDLK_SPACE:
                  {
                     if(m_dialogue->hasDialogue())
                     {
                        m_dialogue->setFastModeEnabled(false);
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
   NPC* npcToActivate = static_cast<NPC*>(m_entityGrid.getAdjacentActor(m_playerActor));
   if(npcToActivate != nullptr)
   {
      npcToActivate->activate();
   }
}
