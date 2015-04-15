/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef TILE_ENGINE_H
#define TILE_ENGINE_H

#include "GameState.h"
#include "MessagePipe.h"
#include "EntityGrid.h"
#include "Camera.h"
#include "Listener.h"
#include "PlayerData.h"
#include "Point2D.h"
#include "PlayerCharacter.h"
#include "Scheduler.h"
#include "TileEngineOverlay.h"
#include "DialogueController.h"

#include <map>
#include <memory>
#include <string>

class NPC;
class ScriptEngine;
class Region;
class DialogueController;
class Task;

struct DebugCommandMessage;
struct MapExitMessage;
struct MapTriggerCallback;
struct MapTriggerMessage;

/**
 * GameState that coordinates all the gameplay involving walking around fields
 * (towns or dungeons).
 * This will be a huge piece of the game, involving coordination of dialogue,
 * NPCs, scripts, battles, and more. See the Requirements Document Wiki for more
 * information about the Tile Engine's purposes.
 *
 * @author Noam Chitayat
 */
class TileEngine final :
   public GameState,
   public messaging::Listener<DebugCommandMessage>,
   public messaging::Listener<MapExitMessage>,
   public messaging::Listener<MapTriggerMessage>,
   public std::enable_shared_from_this<TileEngine>
{
   /** The currently loaded player data. */
   std::shared_ptr<PlayerData> m_playerData;

   /** The current region that the player is in. */
   std::shared_ptr<Region> m_currRegion;

   /** The message pipe used for sending and receiving TileEngine events. */
   messaging::MessagePipe m_messagePipe;

   std::vector<std::pair<std::string, std::unique_ptr<MapTriggerCallback>>> m_triggerScripts;

   /** True iff the TileEngine has already been initialized. */
   bool m_initialized;

   /** The chapter to start, if the game is starting from a new chapter. */
   std::string m_chapterToInitialize;

   /** The location of the player, if the game is starting from a chapter in progress. */
   SaveLocation m_saveLocationToInitialize;

   /** The current map that the player is in. */
   EntityGrid m_entityGrid;

   /** The coroutine scheduler used by the tile engine. */
   Scheduler m_scheduler;

   /** Controller for dialogue and narrations. */
   DialogueController m_dialogue;

   /** The actor representing the player character on the map */
   PlayerCharacter m_playerActor;

   /** The GUI overlaid on top of the map in the tile engine. */
   TileEngineOverlay m_overlay;

   /** A list of all NPCs in the map, identified by their names. */
   std::map<std::string, NPC> m_npcList;

   /** The camera displaying the appropriate subset of the map. */
   Camera m_camera;

   /** An optional Actor target for the camera to follow. */
   const Actor* m_cameraTarget;

   /**
    * Loads a chapter script.
    *
    * @param chapterName The name of the chapter.
    */
   void startChapter(const std::string& chapterName);

   /**
    * Recalculate the camera offset (based on map and window dimensions)
    * in order to center the map and its elements properly.
    */
   void recalculateMapOffsets();

   /**
    * Handles input events specific to the tile engine.
    *
    * @param finishState Returned as true if the input event quit out of the tile engine.
    */
   void handleInputEvents(bool& finishState);

   /**
    * Handles activation of NPCs when the player presses the action key.
    */
   void action();

   /**
    * Updates all NPCs on the map.
    *
    * @param timePassed the amount of time that has passed since the last frame.
    */
   void stepNPCs(long timePassed);

   /**
    * Collects all active actors on the map.
    *
    * @return a vector of all the active actors.
    */
   std::vector<const Actor*> collectActors() const;

   /**
    * Constructor.
    *
    * @param gameContext The context containing the execution stack.
    * @param playerData The currently loaded player data.
    */
   TileEngine(GameContext& gameContext, std::shared_ptr<PlayerData> playerData);

   protected:
      /**
       * Activation of state.
       * Rebinds the TileEngine's message pipe to the player data.
       */
      void activate() override;
      /**
       * Deactivation of state.
       * Unbinds the TileEngine's message pipe from the player data.
       */
      void deactivate() override;

      /**
       * Logic step.
       * Sends time passed to all controllers so that they can update accordingly.
       * Takes user input if there is any.
       */
      bool step(long timePassed) override;

      /**
       * Draw map tiles if a map is loaded in, and then coordinate the drawing
       * of all the controllers and widgets.
       */
      void draw() override;

   public:
      /** Tile size constant */
      static const int TILE_SIZE;

      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack.
       * @param playerData The currently loaded player data.
       * @param chapterName The chapter to begin on activation.
       */
      TileEngine(GameContext& gameContext, std::shared_ptr<PlayerData> playerData, const std::string& chapterName);

      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack.
       * @param playerData The currently loaded player data.
       * @param saveLocation The saved location to return to on activation.
       */
      TileEngine(GameContext& gameContext, std::shared_ptr<PlayerData> playerData, const SaveLocation& saveLocation);

      /**
       * Destructor.
       */
      ~TileEngine() override;

      /**
       * @return The tile engine's coroutine scheduler.
       */
      Scheduler* getScheduler() override;

      /**
       * @return The name of the currently loaded map.
       */
      std::string getMapName() const;

      /**
       * Handler for debug console command messages.
       *
       * @param message The debug command message.
       */
      void receive(const DebugCommandMessage& message) override;

      /**
       * Handler for map location changes.
       *
       * @param message The map exit message.
       */
      void receive(const MapExitMessage& message) override;

      /**
       * Handler for map trigger entry.
       *
       * @param message The map trigger message.
       */
      void receive(const MapTriggerMessage& message) override;

      /**
       * Send a line of dialogue to the DialogueController as a narration.
       *
       * @param narration The line of dialogue to appear as a narration.
       * @param task The ticket of this narration instruction
       */
      void dialogueNarrate(const std::string& narration, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices);

      /**
       * Send a line of dialogue to the DialogueController as speech.
       *
       * @param speech The line of dialogue to appear as character speech.
       * @param task The ticket of this speech instruction
       */
      void dialogueSay(const std::string& speech, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices);

      /**
       * Set a new location for the gameplay to take place in.
       *
       * @param regionName The name of the new region to set.
       * @param mapName The name of the map to use within the region.
       *                By default, uses the first map declared in the region.
       *
       * @return 0 or the yield code for a new map script.
       */
      int startBattle(std::shared_ptr<Task> task);
   
      /**
       * Set a new location for the gameplay to take place in.
       *
       * @param regionName The name of the new region to set.
       * @param mapName The name of the map to use within the region.
       *                By default, uses the first map declared in the region.
       *
       * @return 0 or the yield code for a new map script.
       */
      int setRegion(const std::string& regionName, const std::string& mapName = "");

      /**
       * Set a new map within the current region.
       *
       * @param mapName The name of the map to use within the region.
       *                By default, uses the first map declared in the region.
       *
       * @return 0 or the yield code for a new map script.
       */
      int setMap(std::string mapName = "");

      /**
       * Order the camera to follow a specific actor
       *
       * @param target The actor to follow with the camera
       */
      void followWithCamera(const Actor& target);

      /**
       * Order the camera to stop following its target, if it has one.
       */
      void releaseCamera();

      /**
       * Order the camera to slide from one location of the map to another.
       *
       * @param origin The origin point to start the slide from.
       * @param destination The destination point to slide the camera to.
       * @param speed The speed that the camera should slide with.
       */
      int slideCamera(const geometry::Point2D& origin, const geometry::Point2D& destination, double speed);

      /**
       * Open the save menu.
       *
       * @return 0 or the yield code for the save menu's completion.
       */
      int openSaveMenu();

      /**
       * Get the current location that the camera will reveal.
       */
      geometry::Point2D getCurrentCameraLocation() const;

      /**
       * Add a new NPC with the specified name into the region with the specified spritesheet.
       *
       * @param npcName The name of the npc to add
       * @param spritesheetName The name of the spritesheet to draw the NPC with
       * @param npcLocation The location where we spawn the NPC
       * @param size The size of the new NPC
       * @param direction The direction that the new NPC will face at first
       *
       * @return The created NPC (or nullptr if it could not be placed in the map).
       */
      NPC* addNPC(const std::string& npcName, const std::string& spritesheetName,
                  const geometry::Point2D& npcLocation, const geometry::Size& size,
                  const geometry::Direction direction);

      /**
       * @param npcName The name of the NPC to find.
       *
       * @return The NPC in the current map with the specified name.
       */
      NPC* getNPC(const std::string& npcName);

      /**
       * Adds a listener for a specific trigger zone on the Map, which will
       * execute the specified callback.
       *
       * @param triggerName The name of the trigger to listen for.
       * @param callback The script callback to execute when an Actor sets off the specified trigger.
       */
      void addTriggerListener(const std::string& triggerName, std::unique_ptr<MapTriggerCallback> callback);

      /**
       * @return Whether or not the given actor is the player character.
       */
      bool isPlayerCharacter(const Actor* const actor) const;

      /**
       * @return The player character in the tile engine.
       */
      PlayerCharacter* getPlayerCharacter();
};

#endif
