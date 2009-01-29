#ifndef __DEBUG_H_
   #define __DEBUG_H_

   #include <string>

   #include "Exception.h"

   // Uncomment this macro to turn off music
   //#define MUSIC_OFF

   // Make a nice easy access macro for debug statements in the code
   #define DEBUG(x) DebugUtils::print(debugFlag, x)
   #define DEBUG2(x,y) DebugUtils::print(debugFlag, std::string(x) + y)

   // Macro for exception-related information (should be passed into every exception constructor)
   #define EXCEPTION_INFO __PRETTY_FUNCTION__, __LINE__

   #define T_T(x) throw Exception(EXCEPTION_INFO, x);

   // The debug flags
   #define DEBUG_EXEC_STACK 1<<0     // Execution stack
   #define DEBUG_GAME_STATE 1<<1     // General game state
   #define DEBUG_GRAPHICS   1<<2     // Graphics Utilities
   #define DEBUG_EDWT       1<<3     // ED Widget Toolkit
   #define DEBUG_TITLE      1<<4     // Title Screen
   #define DEBUG_TILE_ENG   1<<5     // Tile Engine
   #define DEBUG_DIA_CONTR  1<<6     // Dialogue Controller
   #define DEBUG_RES_LOAD   1<<6     // Resource Loader
   #define DEBUG_SCRIPT_ENG 1<<7     // Script Engine
   #define DEBUG_AUDIO      1<<8     // Audio functionality

   /**
    *  Provides utilities to help game devs debug the game code,
    *  including a print function for printing error logs.
    *  Also implements functionality for reading program args and setting
    *  debug flags appropriately.
    *
    *  @author Noam Chitayat
    */
   class DebugUtils
   {   /**
        * Debug flags to output on. This variable controls which components of
        * the game will actually output when DEBUG is used.
        */
       static long debugFlags;

       public:

          /**
           * Print a string to the error log if the associated debug flag is active
           *
           * @param str the string to print.
           * @param flag the associated debug flag (usually set at the top of *.cpp files).
           */
          static void print(long flag, std::string str);
   };

#endif
