/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include "Exception.h"

// Make a nice easy access macro for debug statements in the code
#ifdef DEBUG_MODE
   #define DEBUG(str, ...) DebugUtils::print(DEBUG_FLAG, str, ## __VA_ARGS__)
   #define DEBUG_PAUSE DebugUtils::pause();
#else
   #define DEBUG(x, ...)
   #define DEBUG_PAUSE
#endif

// Macro for exception-related information (should be passed into every exception constructor)
#define EXCEPTION_INFO __func__, __LINE__

#define T_T(x) throw Exception(EXCEPTION_INFO, x);

// The debug flags
#define DEBUG_MAIN        1<<0     // Main method (startup code)
#define DEBUG_EXEC_STACK  1<<1     // Execution stack
#define DEBUG_GAME_STATE  1<<2     // General game state
#define DEBUG_GRAPHICS    1<<3     // Graphics Utilities
#define DEBUG_ROCKET      1<<4     // Rocket GUI functionality
#define DEBUG_TITLE       1<<5     // Title Screen
#define DEBUG_TILE_ENG    1<<6     // Tile Engine
#define DEBUG_MENU        1<<7     // In-game Menu
#define DEBUG_BATTLE_ENG  1<<8     // Battle Engine
#define DEBUG_OVERWORLD   1<<9     // Overworld screen
#define DEBUG_DIA_CONTR   1<<10    // Dialogue Controller
#define DEBUG_RES_LOAD    1<<11    // Resource Loader
#define DEBUG_SCRIPT_ENG  1<<12    // Script Engine
#define DEBUG_AUDIO       1<<13    // Audio functionality
#define DEBUG_SCHEDULER   1<<14    // Scheduler/coroutine functionality
#define DEBUG_ACTOR       1<<15    // Actor functionality
#define DEBUG_PATHFINDER  1<<16    // Pathfinding functionality
#define DEBUG_SPRITE      1<<17    // Sprite/spritesheet functionality
#define DEBUG_PLAYER      1<<18    // Player data/Character functionality
#define DEBUG_ENTITY_GRID 1<<19    // Entity Map functionality
#define DEBUG_MESSAGING   1<<20    // Message passing functionality
#define DEBUG_TRANSITIONS 1<<21    // Game state transition functionality
#define DEBUG_METADATA    1<<22    // Game metadata (items, skills)
#define DEBUG_SETTINGS    1<<23    // Game settings and options

#define DEBUG_ALL           ~0    // Turn on all debug output (verbose, but handy)

/**
 *  Provides utilities to help game devs debug the game code,
 *  including a print function for printing error logs.
 *  Also implements functionality for reading program args and setting
 *  debug flags appropriately.
 *
 *  @author Noam Chitayat
 */
class DebugUtils final
{
   /**
    * Debug flags to output on. This variable controls which components of
    * the game will actually output when DEBUG is used.
    */
   static long debugFlags;

   public:
      /**
       * Print a string to the error log if the associated debug flag is active
       *
       * @param flag the associated debug flag (usually set at the top of *.cpp files).
       * @param str the string to print.
       */
      static void print(long flag, std::string str) noexcept;

      /**
       * Print a string to the error log if the associated debug flag is active
       * Uses a format string followed by a variable argument list (treat like printf)
       *
       * @param flag the associated debug flag (usually set at the top of *.cpp files).
       * @param fmt the format string to print
       */
      static void print(long flag, const char* fmt, ...) noexcept;

      /**
       * Standard debug pausing mechanism.
       * Print out "Press enter to continue" and wait for input.
       */
      static void pause() noexcept;
};

#endif
