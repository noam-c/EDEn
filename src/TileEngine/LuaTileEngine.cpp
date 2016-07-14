/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "LuaTileEngine.h"

#include "EdenApiMacros.h"
#include "EnumUtils.h"
#include "MapTriggerCallback.h"
#include "Direction.h"
#include "NPC.h"
#include "Point2D.h"
#include "ScriptEngine.h"
#include "ScriptUtilities.h"
#include "Size.h"
#include "TileEngine.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_SCRIPT_ENG

EDEN_API_START_FUNC(TileEngineL_AddNPC)
   EDEN_API_CONTEXT(TileEngine, tileEngine);
   EDEN_API_PARAM(std::string, name);
   EDEN_API_PARAM(std::string, spritesheet);
   EDEN_API_PARAM(int, x);
   EDEN_API_PARAM(int, y);
   EDEN_API_OPT_PARAM(unsigned int, width, 32);
   EDEN_API_OPT_PARAM(unsigned int, height, 32);
   EDEN_API_OPT_PARAM(geometry::Direction, direction, geometry::Direction::DOWN);
{
   DEBUG("Adding NPC %s with spritesheet %s", name.c_str(), spritesheet.c_str());
   DEBUG("NPC Location will be (%d, %d)", x, y);

   auto npc = tileEngine->addNPC(name, spritesheet, {x, y}, {width, height}, direction);

   EDEN_API_RETURN(npc);
}
EDEN_API_END_FUNC()

EDEN_API_START_FUNC(TileEngineL_AddTriggerListener)
   EDEN_API_CONTEXT(TileEngine, tileEngine);
   EDEN_API_PARAM(std::string, name);
{
   lua_pushstring(luaVM, "handler");
   lua_rawget(luaVM, 2);

   tileEngine->addTriggerListener(name, std::unique_ptr<MapTriggerCallback>(new MapTriggerCallback(luaVM)));
}
EDEN_API_END_FUNC()

EDEN_API_START_FUNC(TileEngineL_GetNPC)
   EDEN_API_CONTEXT(TileEngine, tileEngine);
   EDEN_API_PARAM(std::string, name);
{
   EDEN_API_RETURN(tileEngine->getNPC(name));
}
EDEN_API_END_FUNC()

EDEN_API_START_FUNC(TileEngineL_FollowWithCamera)
   EDEN_API_CONTEXT(TileEngine, tileEngine)
   EDEN_API_PARAM(GridActor*, target)
{
   tileEngine->followWithCamera(*target);
}
EDEN_API_END_FUNC()

EDEN_API_START_FUNC(TileEngineL_ReleaseCamera)
   EDEN_API_CONTEXT(TileEngine, tileEngine)
   EDEN_API_NO_PARAMS()
{
   tileEngine->releaseCamera();
}
EDEN_API_END_FUNC()

static int TileEngineL_SlideCamera(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   int destinationX;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "destinationX", destinationX))
   {
      return lua_error(luaVM);
   }

   int destinationY;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "destinationY", destinationY))
   {
      return lua_error(luaVM);
   }

   const geometry::Point2D destination(destinationX, destinationY);

   double speed;
   if(!ScriptUtilities::getParameter(luaVM, 2, 5, "speed", speed) || speed <= 0.0f)
   {
      speed = 0.1f;
   }

   auto slideWork = [tileEngine, destination, speed](const std::shared_ptr<Task>& task)
   {
      tileEngine->slideCamera(destination, speed, task);
   };
   
   auto scriptEngine = ScriptEngine::getScriptEngineForVM(luaVM);
   scriptEngine->scheduleWork(slideWork, true);
}

static int TileEngineL_OpenSaveMenu(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   auto saveMenuWork = [tileEngine](const std::shared_ptr<Task>& task)
   {
      tileEngine->openSaveMenu(task);
   };

   auto scriptEngine = ScriptEngine::getScriptEngineForVM(luaVM);
   return scriptEngine->scheduleWork(saveMenuWork, true /*waitUntilFinished*/);
}

EDEN_API_START_FUNC(TileEngineL_TilesToPixels)
   EDEN_API_CONTEXT(TileEngine, tileEngine)
   EDEN_API_PARAM(int, tiles)
{
   EDEN_API_RETURN(TileEngine::TILE_SIZE * tiles);
}
EDEN_API_END_FUNC()

static luaL_Reg tileEngineMetatable[] =
{
   { "addNPC", TileEngineL_AddNPC },
   { "addTriggerListener", TileEngineL_AddTriggerListener },
   { "getNPC", TileEngineL_GetNPC },
   { "lockCameraToTarget", TileEngineL_FollowWithCamera },
   { "unlockCamera", TileEngineL_ReleaseCamera },
   { "slideCamera", TileEngineL_SlideCamera },
   { "openSaveMenu", TileEngineL_OpenSaveMenu },
   { "tilesToPixels", TileEngineL_TilesToPixels },
   { nullptr, nullptr }
};

EDEN_API_CLASS(TileEngine, tileEngineMetatable);
