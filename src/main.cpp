/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "GraphicsUtil.h"
#include "Settings.h"
#include "GameContext.h"
#include "ExecutionStack.h"
#include "MainMenu.h"
#include "ResourceLoader.h"
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
      Settings::load();
      GraphicsUtil::getInstance();
      DEBUG("Initializing execution stack.");
      GameContext gameContext;
      DEBUG("Pushing Main Menu state.");
      gameContext.getExecutionStack().pushState(new MainMenu(gameContext));
      DEBUG("Beginning game execution.");
      gameContext.getExecutionStack().execute();

      DEBUG("Game is finished. Freeing resources and destroying singletons.");
      ResourceLoader::freeAll();
      GraphicsUtil::destroy();
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
