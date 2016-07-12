/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "LuaTileEngine.h"

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

static int TileEngineL_AddNPC(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string npcName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "name", npcName))
   {
      return lua_error(luaVM);
   }

   std::string spritesheetName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "spritesheet", spritesheetName))
   {
      return lua_error(luaVM);
   }

   int x;
   if(!ScriptUtilities::getParameter(luaVM, 2, 3, "x", x))
   {
      return lua_error(luaVM);
   }

   int y;
   if(!ScriptUtilities::getParameter(luaVM, 2, 4, "y", y))
   {
      return lua_error(luaVM);
   }

   unsigned int width;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "width", width))
   {
      width = 32;
   }

   unsigned int height;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "height", height))
   {
      height = 32;
   }

   int directionValue;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "direction", directionValue))
   {
      directionValue = EnumUtils::toNumber(geometry::Direction::DOWN);
   }

   DEBUG("Adding NPC %s with spritesheet %s", npcName.c_str(), spritesheetName.c_str());
   DEBUG("NPC Location will be (%d, %d)", x, y);

   const geometry::Direction direction = static_cast<geometry::Direction>(directionValue);

   auto npc = tileEngine->addNPC(npcName, spritesheetName, {x, y}, {width, height}, direction);

   luaW_push<Actor>(luaVM, npc);
   return 1;
}

static int TileEngineL_AddTriggerListener(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string triggerName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "name", triggerName))
   {
      return lua_error(luaVM);
   }

   lua_pushstring(luaVM, "handler");
   lua_rawget(luaVM, 2);

   tileEngine->addTriggerListener(triggerName, std::unique_ptr<MapTriggerCallback>(new MapTriggerCallback(luaVM)));

   return 0;
}

static int TileEngineL_GetNPC(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string npcName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "name", npcName))
   {
      return lua_error(luaVM);
   }

   NPC* npc = tileEngine->getNPC(npcName);
   luaW_push<NPC>(luaVM, npc);
   return 1;
}

static int TileEngineL_FollowWithCamera(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   Actor* target = nullptr;
   if(!ScriptUtilities::getParameter<Actor>(luaVM, 2, 1, "target", target))
   {
      return lua_error(luaVM);
   }

   tileEngine->followWithCamera(*target);
   return 0;
}

static int TileEngineL_ReleaseCamera(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   tileEngine->releaseCamera();
   return 0;
}

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

static int TileEngineL_TilesToPixels(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   int tileNum;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "tiles", tileNum))
   {
      return lua_error(luaVM);
   }

   lua_pushnumber(luaVM, TileEngine::TILE_SIZE * tileNum);
   return 1;
}

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

void luaopen_TileEngine(lua_State* luaVM)
{
   luaW_register<TileEngine>(luaVM, "TileEngine", nullptr, tileEngineMetatable, nullptr, nullptr);
}
