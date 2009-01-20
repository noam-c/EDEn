#ifndef __TILE_ENGINE_H_
#define __TILE_ENGINE_H_

#include "GraphicsUtil.h"
#include "GameState.h"
#include "ResourceLoader.h"
#include "Map.h"
#include "DialogueController.h"
#include "OpenGLTTF.h"

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

   /** Controller for dialogue and narrations. */
   DialogueController* dialogue;

   public:
      /**
       * Constructor.
       *
       * Initializes widgets and controllers
       */
      TileEngine();

      /**
       * Destructor.
       */
      ~TileEngine();

      /**
       * Draw map tiles if a map is loaded in, and then coordinate the drawing
       * of all the controllers and widgets.
       */
      void draw();

      /**
       * Logic step.
       * Sends time passed to all controllers so that they can update accordingly.
       * Takes user input if there is any. 
       */
      bool step();

      /**
       * Send a line of dialogue to the DialogueController as a narration.
       *
       * @param narration The line of dialogue to appear as a narration.
       */
      void dialogueNarrate(const char* narration);

      /**
       * Send a line of dialogue to the DialogueController as speech.
       *
       * @param speech The line of dialogue to appear as character speech.
       */
      void dialogueSay(const char* speech);
      
      /**
       * Set a new location for the gameplay to take place in.
       *
       * @param regionName The name of the new region to set.
       * @param mapName The name of the map to use within the region.
       *                By default, uses the first map declared in the region.
       */
      void setRegion(std::string regionName, std::string mapName = "");
};

#endif
