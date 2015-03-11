/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "TileEngine.h"

#include "stdlib.h"
#include <Rocket/Core.h>
#include <algorithm>
#include <utility>
#include <tuple>
#include <SDL.h>

#include "DebugCommandMessage.h"
#include "ScriptEngine.h"
#include "NPC.h"
#include "PlayerData.h"
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
#include "ExecutionStack.h"
#include "CameraSlider.h"
#include "RandomTransitionGenerator.h"
#include "HomeMenu.h"
#include "SaveMenu.h"
#include "ScreenTexture.h"
#include "MapTriggerCallback.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_TILE_ENG

const int TileEngine::TILE_SIZE = 32;

TileEngine::TileEngine(GameContext& gameContext, std::shared_ptr<PlayerData> playerData) :
   GameState(gameContext, GameStateType::FIELD, "TileEngine"),
   m_playerData(std::move(playerData)),
   m_initialized(false),
   m_entityGrid(*this, m_messagePipe),
   m_dialogue(getScriptEngine()),
   m_playerActor(m_messagePipe, m_entityGrid, *m_playerData),
   m_overlay(m_messagePipe, *m_playerData, getMetadata(), getStateType(), *m_rocketContext, m_dialogue)
{
   m_messagePipe.registerListener<DebugCommandMessage>(this);
   m_messagePipe.registerListener<MapExitMessage>(this);
   m_messagePipe.registerListener<MapTriggerMessage>(this);

   m_cameraTarget = &m_playerActor;

   m_dialogue.initialize(m_scheduler, m_overlay.getDialogueBox());
}

TileEngine::TileEngine(GameContext& gameContext, std::shared_ptr<PlayerData> playerData, const std::string& chapterName) :
   TileEngine(gameContext, std::move(playerData))
{
   m_chapterToInitialize = chapterName;
}

TileEngine::TileEngine(GameContext& gameContext, std::shared_ptr<PlayerData> playerData, const SaveLocation& saveLocation) :
   TileEngine(gameContext, std::move(playerData))
{
   m_saveLocationToInitialize = saveLocation;
}

TileEngine::~TileEngine()
{
   m_playerData->unbindMessagePipe();
}

void TileEngine::startChapter(const std::string& chapterName)
{
   getScriptEngine().runChapterScript(chapterName, m_scheduler);
}

Scheduler* TileEngine::getScheduler()
{
   return &m_scheduler;
}

std::string TileEngine::getMapName() const
{
   return m_entityGrid.getMapName();
}

void TileEngine::receive(const DebugCommandMessage& message)
{
   getScriptEngine().runScriptString(message.command);
}

void TileEngine::receive(const MapExitMessage& message)
{
   if(m_playerActor.isActive())
   {
      m_playerActor.removeFromMap();
      std::string exitedMap = m_entityGrid.getMapName();
      std::string enteredMap = message.mapExit.getNextMap();
      DEBUG("Exit signal received: Exiting %s and entering %s", exitedMap.c_str(), enteredMap.c_str());
      setMap(enteredMap);
      const geometry::Point2D& entryPoint = m_entityGrid.getMapEntrance(exitedMap);
      m_playerActor.addToMap(entryPoint);
      followWithCamera(m_playerActor);
   }
}

void TileEngine::receive(const MapTriggerMessage& message)
{
   for(auto& triggerIter : m_triggerScripts)
   {
      if(triggerIter.first == message.triggerZone.getName())
      {
         auto triggeringActor = message.triggeringActor;
         auto& triggerCallback = *triggerIter.second;

         if(isPlayerCharacter(triggeringActor))
         {
            triggerCallback(&m_playerActor);
         }
         else if(triggeringActor)
         {
            triggerCallback(getNPC(triggeringActor->getName()));
         }
      }
   }
}

void TileEngine::dialogueNarrate(const std::string& narration, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices)
{
   m_dialogue.narrate(narration, task, choices);
}

void TileEngine::dialogueSay(const std::string& speech, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices)
{
   m_dialogue.say(speech, task, choices);
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
   m_triggerScripts.clear();
   m_npcList.clear();
   m_playerActor.removeFromMap();

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
   return getScriptEngine().runMapScript(m_currRegion->getName(), mapName, m_scheduler);
}

void TileEngine::followWithCamera(const Actor& target)
{
   m_cameraTarget = &target;
}

void TileEngine::releaseCamera()
{
   m_cameraTarget = nullptr;
}

int TileEngine::slideCamera(const geometry::Point2D& origin, const geometry::Point2D& destination, double speed)
{
   if(speed > 0)
   {
      auto slider = std::make_shared<CameraSlider>(m_camera, origin, destination, speed);
      m_cameraTarget = nullptr;
      m_scheduler.start(slider);
      return m_scheduler.join(slider);
   }

   return 0;
}

int TileEngine::openSaveMenu()
{
   SaveLocation location;
   location.region = m_currRegion->getName();
   location.map = m_entityGrid.getMapName();
   location.coords = m_playerActor.getLocation();
   location.direction = m_playerActor.getDirection();

   auto saveMenu = std::make_shared<SaveMenu>(m_gameContext, m_playerData, location);
   getExecutionStack()->pushState(saveMenu);
   return 0;
}

geometry::Point2D TileEngine::getCurrentCameraLocation() const
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
   const geometry::Size mapPixelBounds = m_entityGrid.hasMapData() ?
      m_entityGrid.getMapBounds().getSize() * TILE_SIZE :
      geometry::Size();

   const int totalUsableHeight = GraphicsUtil::getInstance()->getHeight() - m_overlay.getShortcutBarHeight();
   const geometry::Size screenSize(GraphicsUtil::getInstance()->getWidth(), totalUsableHeight);
   m_camera.setViewBounds(screenSize, mapPixelBounds);
}

NPC* TileEngine::addNPC(const std::string& npcName, const std::string& spritesheetName, const geometry::Point2D& npcLocation, const geometry::Size& size, const Direction direction)
{
   NPC* npcToAdd = nullptr;

   if(m_entityGrid.isAreaFree(geometry::Rectangle(npcLocation, size)))
   {
      auto insertResult = m_npcList.emplace(
                          std::piecewise_construct,
                          std::forward_as_tuple(npcName),
                          std::forward_as_tuple(
                              getScriptEngine(), m_scheduler, npcName, direction,
                              spritesheetName, m_messagePipe, m_entityGrid,
                              m_currRegion->getName(), npcLocation, size));

      if(insertResult.second)
      {
         npcToAdd = &insertResult.first->second;
         m_entityGrid.addActor(npcToAdd, npcLocation);
      }
      else
      {
         DEBUG("Failed to insert NPC into list of NPCs");
      }
   }
   else
   {
      DEBUG("Cannot place NPC at this location; something is in the way.");
   }

   return npcToAdd;
}

NPC* TileEngine::getNPC(const std::string& npcName)
{
   const auto& npcIterator = m_npcList.find(npcName);
   if(npcIterator != m_npcList.end())
   {
      return &npcIterator->second;
   }

   return nullptr;
}

void TileEngine::addTriggerListener(const std::string& triggerName, std::unique_ptr<MapTriggerCallback> callback)
{
   m_triggerScripts.emplace_back(triggerName, std::move(callback));
}

bool TileEngine::isPlayerCharacter(const Actor* const actor) const
{
   return &m_playerActor == actor;
}

PlayerCharacter* TileEngine::getPlayerCharacter()
{
   return &m_playerActor;
}

void TileEngine::activate()
{
   GameState::activate();
   m_overlay.refresh();
   recalculateMapOffsets();
   m_playerData->bindMessagePipe(&m_messagePipe);
   getScriptEngine().setTileEngine(shared_from_this());
   getScriptEngine().setPlayerData(m_playerData);

   if(!m_initialized)
   {
      if(!m_chapterToInitialize.empty())
      {
         startChapter(m_chapterToInitialize);
      }
      else
      {
         if(!m_saveLocationToInitialize.isValid())
         {
            T_T("Invalid save data. Expected valid chapter name or save point data.");
         }

         setRegion(m_saveLocationToInitialize.region, m_saveLocationToInitialize.map);
         m_playerActor.addToMap(m_saveLocationToInitialize.coords);
         m_playerActor.setDirection(m_saveLocationToInitialize.direction);
         followWithCamera(m_playerActor);
      }

      m_initialized = true;
   }
}

void TileEngine::deactivate()
{
   getScriptEngine().setPlayerData(nullptr);
   getScriptEngine().setTileEngine(nullptr);
   m_playerData->unbindMessagePipe();
   GameState::deactivate();
   m_overlay.refresh();
   recalculateMapOffsets();
}

void TileEngine::stepNPCs(long timePassed)
{
   for(auto& iter : m_npcList)
   {
      iter.second.step(timePassed);
   }
}

std::vector<const Actor*> TileEngine::collectActors() const
{
   std::vector<const Actor*> actors;
   actors.reserve(m_npcList.size() + 1);
   for(auto& iter : m_npcList)
   {
      actors.push_back(&(iter.second));
   }

   if(m_playerActor.isActive())
   {
      actors.push_back(&m_playerActor);
   }

   return actors;
}

void TileEngine::draw()
{
   // Collect the drawable actors and sort them by their y-location (in tiles)
   auto actors = collectActors();

   GraphicsUtil::getInstance()->clearBuffer();

   m_camera.apply();
      if(!m_entityGrid.hasMapData())
      {
         // Draw all the sprites
         for(const auto& nextActorToDraw : actors)
         {
            nextActorToDraw->draw();
         }
      }
      else
      {
         std::sort(
            actors.begin(),
            actors.end(),
            [](const Actor* lhs, const Actor* rhs)
            {
               return lhs->getLocation().y + lhs->getSize().height < rhs->getLocation().y + rhs->getSize().height;
            }
         );

         const unsigned int mapHeight = m_entityGrid.getMapBounds().getHeight();
         for(int row = 0; row < mapHeight; ++row)
         {
            // Start by drawing a row of the background layers, if the map exists
            if(m_entityGrid.hasMapData())
            {
               m_entityGrid.drawBackground(row);
            }
         }

         auto nextActorToDraw = actors.begin();

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
   m_scheduler.runCoroutines(timePassed);

   handleInputEvents(done);

   m_playerActor.step(timePassed);
   m_entityGrid.step(timePassed);

   stepNPCs(timePassed);

   if(m_cameraTarget &&
      (isPlayerCharacter(m_cameraTarget) || m_playerActor.isActive()))
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
         case SDL_MOUSEBUTTONDOWN:
         {
            const geometry::Point2D mouseClickLocation(event.button.x, event.button.y);
            if(m_camera.isPointWithinViewport(mouseClickLocation))
            {
               const geometry::Point2D pointWithinScene = m_camera.getPointWithinScene(mouseClickLocation);

               if(m_entityGrid.isAreaFree(geometry::Rectangle(pointWithinScene, m_playerActor.getSize())))
               {
                  m_playerActor.move(pointWithinScene, nullptr);
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
                  if(!m_overlay.isDebugConsoleVisible())
                  {
                     if(m_dialogue.hasDialogue())
                     {
                        m_dialogue.setFastModeEnabled(true);

                        if (!event.key.repeat)
                        {
                           m_dialogue.nextLine();
                        }
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
                  m_overlay.toggleDebugConsole();
                  return;
               }
               case SDLK_TAB:
               {
                  if(!m_overlay.isDebugConsoleVisible())
                  {
                     auto menu = std::make_shared<HomeMenu>(m_gameContext, *m_playerData);
                     getExecutionStack()->pushState(menu, RandomTransitionGenerator::create(m_gameContext, ScreenTexture::create(*this), ScreenTexture::create(*menu)));
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
            if(!m_overlay.isDebugConsoleVisible())
            {
               switch(event.key.keysym.sym)
               {
                  case SDLK_SPACE:
                  {
                     if(m_dialogue.hasDialogue())
                     {
                        m_dialogue.setFastModeEnabled(false);
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
   auto npcToActivate = static_cast<NPC*>(m_entityGrid.getAdjacentActor(&m_playerActor));
   if(npcToActivate)
   {
      npcToActivate->activate();
   }
}
