/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef TILE_ENGINE_H
#define TILE_ENGINE_H

#include "GameState.h"

#include "MessagePipe.h"
#include "DebugConsoleWindow.h"
#include "EntityGrid.h"
#include "Camera.h"
#include "Listener.h"
#include "PlayerData.h"
#include "ShortcutBar.h"
#include "Point2D.h"

#include <map>
#include <string>

class NPC;
class ScriptEngine;
class PlayerCharacter;
class Region;
class DialogueController;
class Task;

/**
 * GameState that coordinates all the gameplay involving walking around fields
 * (towns or dungeons).
 * This will be a huge piece of the game, involving coordination of dialogue,
 * NPCs, scripts, battles, and more. See the Requirements Document Wiki for more
 * information about the Tile Engine's purposes.
 *
 * @author Noam Chitayat
 */
class TileEngine: public GameState, public messaging::Listener<MapExitMessage>
{
   /** The current region that the player is in. */
   Region* currRegion;
   
   messaging::MessagePipe messagePipe;

   /** The debug console window to be used for diagnostics. */
   DebugConsoleWindow consoleWindow;

   /** The current map that the player is in. */
   EntityGrid entityGrid;

   /** The player data */
   PlayerData playerData;

   /** The window containing the player's shortcuts. */
   ShortcutBar shortcutBar;

   /** Controller for dialogue and narrations. */
   DialogueController* dialogue;

   /** The coroutine scheduler used by the tile engine. */
   Scheduler* scheduler;

   /** The actor representing the player character on the map */
   PlayerCharacter* playerActor;

   /** A list of all NPCs in the map, identified by their names. */
   std::map<std::string, NPC*> npcList;

   /** The camera displaying the appropriate subset of the map. */
   Camera camera;

   /** An optional Actor target for the camera to follow. */
   const Actor* cameraTarget;

   /**
    * Loads new player data.
    *
    * @param path The path to the player data file.
    */
   void loadPlayerData(const std::string& path);
   
   /**
    * Loads a chapter script.
    *
    * @param chapterName The name of the chapter.
    */
   void startChapter(const std::string& chapterName);
   
   /**
    * Toggles the debug console on or off.
    */
   void toggleDebugConsole();

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
    * Clears all the NPCs on the map.
    */
   void clearNPCs();

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
   std::vector<Actor*> collectActors() const;

   protected:
      /**
       * Activation of state.
       * Rebinds the TileEngine's message pipe to the player data.
       */
      void activate();

      /**
       * Logic step.
       * Sends time passed to all controllers so that they can update accordingly.
       * Takes user input if there is any. 
       */
      bool step(long timePassed);

      /**
       * Draw map tiles if a map is loaded in, and then coordinate the drawing
       * of all the controllers and widgets.
       */
      void draw();

   public:
      /** Tile size constant */
      static const int TILE_SIZE;

      /**
       * Constructor.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param chapterName The name of the chapter to load after construction
       * @param playerDataPath The path to the player's data.
       */
      TileEngine(GameContext& gameContext, const std::string& chapterName, const std::string& playerDataPath = "");

      /**
       * Destructor.
       */
      ~TileEngine();

      /**
       * @return The tile engine's coroutine scheduler.
       */
      Scheduler* getScheduler() const;

      /**
       * @return The name of the currently loaded map.
       */
      std::string getMapName() const;

      /**
       * Handler for map location changes.
       *
       * @param message The map exit message.
       */
      void receive(const MapExitMessage& message);

      /**
       * Send a line of dialogue to the DialogueController as a narration.
       *
       * @param narration The line of dialogue to appear as a narration.
       * @param task The ticket of this narration instruction
       */
      void dialogueNarrate(const std::string& narration, Task* task);

      /**
       * Send a line of dialogue to the DialogueController as speech.
       *
       * @param speech The line of dialogue to appear as character speech.
       * @param task The ticket of this speech instruction
       */
      void dialogueSay(const std::string& speech, Task* task);
      
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
      void followWithCamera(const Actor* target);

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
      void slideCamera(const shapes::Point2D& origin, const shapes::Point2D& destination, double speed);

      /**
       * Get the current location that the camera will reveal.
       */
      shapes::Point2D getCurrentCameraLocation() const;

      /**
       * Add a new NPC with the specified name into the region with the specified spritesheet.
       *
       * @param npcName The name of the npc to add
       * @param spritesheetName The name of the spritesheet to draw the NPC with
       * @param npcLocation The location where we spawn the NPC
       * @param size The size of the new NPC
       * @param direction The direction that the new NPC will face at first
       *
       * @return The created NPC (or NULL if it could not be placed in the map).
       */
      NPC* addNPC(const std::string& npcName, const std::string& spritesheetName,
                  const shapes::Point2D& npcLocation, const shapes::Size& size,
                  const MovementDirection direction);

      /**
       * @param npcName The name of the NPC to find.
       *
       * @return The NPC in the current map with the specified name.
       */
      NPC* getNPC(const std::string& npcName) const;

      /**
       * @return The player character in the tile engine.
       */
      PlayerCharacter* getPlayerCharacter() const;
};

#endif
