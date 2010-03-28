#ifndef __DEBUG_H_
#define __DEBUG_H_

#include <string>
#include "Exception.h"

// Make a nice easy access macro for debug statements in the code
#ifdef DEBUG_MODE
   #define DEBUG(x, ...) DebugUtils::print(debugFlag, x, ## __VA_ARGS__)
   #define DEBUG_PAUSE DebugUtils::pause();
#else
   #define DEBUG(x, ...)
   #define DEBUG_PAUSE
#endif

#ifndef __PRETTY_FUNCTION__
   #define __PRETTY_FUNCTION__ __FILE__
#endif

// Macro for exception-related information (should be passed into every exception constructor)
#define EXCEPTION_INFO __PRETTY_FUNCTION__, __LINE__

#define T_T(x) throw Exception(EXCEPTION_INFO, x);

// The debug flags
#define DEBUG_MAIN       1<<0     // Main method (startup code)
#define DEBUG_EXEC_STACK 1<<1     // Execution stack
#define DEBUG_GAME_STATE 1<<2     // General game state
#define DEBUG_GRAPHICS   1<<3     // Graphics Utilities
#define DEBUG_EDWT       1<<4     // ED Widget Toolkit
#define DEBUG_TITLE      1<<5     // Title Screen
#define DEBUG_TILE_ENG   1<<6     // Tile Engine
#define DEBUG_DIA_CONTR  1<<7     // Dialogue Controller
#define DEBUG_RES_LOAD   1<<8     // Resource Loader
#define DEBUG_SCRIPT_ENG 1<<9     // Script Engine
#define DEBUG_AUDIO      1<<10    // Audio functionality
#define DEBUG_SCHEDULER  1<<11    // Scheduler/coroutine functionality
#define DEBUG_NPC        1<<12    // NPC functionality
#define DEBUG_SPRITE     1<<13    // Sprite/spritesheet functionality

#define DEBUG_ALL           ~0    // Turn on all debug output (verbose, but handy)

/**
 *  Provides utilities to help game devs debug the game code,
 *  including a print function for printing error logs.
 *  Also implements functionality for reading program args and setting
 *  debug flags appropriately.
 *
 *  @author Noam Chitayat
 */
class DebugUtils
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
      static void print(long flag, std::string str);

      /**
       * Print a string to the error log if the associated debug flag is active
       * Uses a format string followed by a variable argument list (treat like printf)
       *
       * @param flag the associated debug flag (usually set at the top of *.cpp files).
       * @param fmt the format string to print
       */
      static void print(long flag, const char* fmt, ...);

      /**
       * Standard debug pausing mechanism.
       * Print out "Press enter to continue" and wait for input.
       */
      static void pause();
};

#endif
