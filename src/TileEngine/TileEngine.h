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
   /** Time since the first logic step of the TileEngine instance */
   unsigned long time;

   /**
    * The current region that the player is in.
    */
   Region* currRegion;

   /** 
    * The current map that the player is in. 
    */
   Map* currMap;

   /** Controller for dialogue and narrations */
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

      void dialogueNarrate(const char* speech);

      void dialogueSay(const char* speech);
      
      void setRegion(std::string regionName, std::string mapName = "");
};

#endif
