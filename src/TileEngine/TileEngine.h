#ifndef __TILE_ENGINE_H_
#define __TILE_ENGINE_H_

#include "GameState.h"

#include <map>
#include <string>

class NPC;
class Scheduler;
class ScriptEngine;
class PlayerCharacter;
class Region;
class Map;
class DialogueController;
class Task;

namespace edwt
{
   class DebugConsoleWindow;
};

/**
 * GameState that coordinates all the gameplay involving walking around fields
 * (towns or dungeons).
 * This will be a huge piece of the game, involving coordination of dialogue,
 * NPCs, scripts, battles, and more. See the Requirements Document Wiki for more
 * information about the Tile Engine's purposes.
 *
 * @author Noam Chitayat
 */
class TileEngine: public GameState
{
   /** Time since the first logic step of the TileEngine instance. */
   unsigned long time;

   /** The current region that the player is in. */
   Region* currRegion;

   /** The current map that the player is in. */
   Map* currMap;

   /** The debug console window to be used for diagnostics. */
   edwt::DebugConsoleWindow* consoleWindow;

   /** Controller for dialogue and narrations. */
   DialogueController* dialogue;

   /** The Scripting Engine used for the engine's scripting. */
   ScriptEngine* scriptEngine;

   /** The Thread scheduler used by the tile engine. */
   Scheduler* scheduler;

   /** The player character */
   PlayerCharacter* player;

   /** A list of all NPCs in the map, identified by their names. */
   std::map<std::string, NPC*> npcList;

   /** The x-offset to draw elements of the map at. */
   int xMapOffset;

   /** The y-offset to draw elements of the map at. */
   int yMapOffset;

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
    * @return true iff the x-y coordinate is within the map
    */
   bool withinMap(int x, int y);

   protected:
      /**
       * Logic step.
       * Sends time passed to all controllers so that they can update accordingly.
       * Takes user input if there is any. 
       */
      bool step();

      /**
       * Draw map tiles if a map is loaded in, and then coordinate the drawing
       * of all the controllers and widgets.
       */
      void draw();

   public:
      /** Tile size constant */
      static const int TILE_SIZE;

      /**
       * Constructor. Initializes widgets and controllers
       *
       * @param chapterName The name of the chapter to load after construction
       */
      TileEngine(const std::string& chapterName);

      /**
       * @return The name of the currently loaded map.
       */
      std::string getMapName();

      /**
       * Updates all NPCs on the map.
       *
       * @param timePassed the amount of time that has passed since the last frame. 
       */
      void stepNPCs(long timePassed);

      /**
       * Draws all NPCs on the map.
       */
      void drawNPCs();

      /**
       * Send a line of dialogue to the DialogueController as a narration.
       *
       * @param narration The line of dialogue to appear as a narration.
       * @param task The ticket of this narration instruction
       */
      void dialogueNarrate(const char* narration, Task* task);

      /**
       * Send a line of dialogue to the DialogueController as speech.
       *
       * @param speech The line of dialogue to appear as character speech.
       * @param task The ticket of this speech instruction
       */
      void dialogueSay(const char* speech, Task* task);
      
      /**
       * Set a new location for the gameplay to take place in.
       *
       * @param regionName The name of the new region to set.
       * @param mapName The name of the map to use within the region.
       *                By default, uses the first map declared in the region.
       *
       * @return true iff the region was successfully set, and a map successfully loaded
       */
      bool setRegion(const std::string& regionName, const std::string& mapName = "");

      /**
       * Set a new map within the current region.
       *
       * @param mapName The name of the map to use within the region.
       *                By default, uses the first map declared in the region.
       */
      void setMap(std::string mapName = "");

      /**
       * Add a new NPC with the specified name into the region with the specified spritesheet.
       *
       * @param npcName The name of the npc to add
       * @param spritesheetName The name of the spritesheet to draw the NPC with
       * @param x The x location where we spawn the NPC
       * @param y The y location where we spawn the NPC
       */
      void addNPC(const std::string& npcName, const std::string& spritesheetName, int x, int y);

      /**
       * Move an NPC with the specified name to the specified location.
       *
       * @param npcName The name of the npc to move
       * @param x The x location to move the NPC to
       * @param y The y location to move the NPC to
       */
      void moveNPC(const std::string& npcName, int x, int y);

      /**
       * Change the sprite frame of the specified NPC.
       *
       * @param npcName The name of the NPC to change.
       * @param animationName The name of the animation to change to.
       */
      void setNPCAnimation(const std::string& npcName, const std::string& animationName);

      /**
       * Change the sprite frame of the specified NPC.
       *
       * @param npcName The name of the NPC to change.
       * @param frameName The name of the frame to change to.
       */
      void setNPCSprite(const std::string& npcName, const std::string& frameName);

      /**
       * Change the spritesheet of the specified NPC.
       *
       * @param npcName The name of the NPC to change.
       * @param spritesheetName The name of the spritesheet to use.
       */
      void changeNPCSpritesheet(const std::string& npcName, const std::string& spritesheetName);

      /**
       * Destructor.
       */
      ~TileEngine();
};

#endif
