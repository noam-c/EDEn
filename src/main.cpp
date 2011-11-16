#include "GraphicsUtil.h"
#include "ScriptEngine.h"
#include "ExecutionStack.h"
#include "MainMenu.h"
#include "ResourceLoader.h"
#include "guichan.hpp"
#include <iostream>
#include <fstream>

#include "SDL.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MAIN;

/**
 * The main function.
 * Creates the graphics utilities, pushes a title screen onto the ExecutionStack,
 * and executes it. Afterwards, destroys graphics utilities and we're done.
 */
int main (int argc, char *argv[])
{  
   try
   {
      GraphicsUtil::getInstance();
      DEBUG("Initializing execution stack.");
      ExecutionStack stack;
      DEBUG("Pushing Main Menu state.");
      stack.pushState(new MainMenu(stack));
      DEBUG("Beginning game execution.");
      stack.execute();

      DEBUG("Game is finished. Freeing resources and destroying singletons.");
      ResourceLoader::freeAll();
      GraphicsUtil::destroy();
   }
   catch (gcn::Exception& e)
   {
      DEBUG("Uncaught Guichan exception: \n%s", e.getMessage().c_str());
      return 1;
   }
   catch(Exception& e)
   {
      DEBUG("Uncaught game exception: \n%s", e.getMessage().c_str());
      return 1;
   }
   catch(std::exception& e)
   {
      DEBUG("Uncaught STL exception: \n%s", e.what());
      return 1;
   }
   catch(...)
   {
      DEBUG("Uncaught general exception.");
      return 1;
   }

	return 0;
}
