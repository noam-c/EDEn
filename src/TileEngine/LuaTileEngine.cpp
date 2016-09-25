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
#include "NPCSpawnMarker.h"
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

   NPCSpawnMarker marker;

   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "name", marker.name))
   {
      return lua_error(luaVM);
   }

   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "spritesheet", marker.spritesheet))
   {
      return lua_error(luaVM);
   }

   if(!ScriptUtilities::getParameter(luaVM, 2, 3, "x", marker.location.x))
   {
      return lua_error(luaVM);
   }

   if(!ScriptUtilities::getParameter(luaVM, 2, 4, "y", marker.location.y))
   {
      return lua_error(luaVM);
   }

   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "width", marker.size.width))
   {
      marker.size.width = 32;
   }

   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "height", marker.size.height))
   {
      marker.size.height = 32;
   }

   int directionValue;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "direction", directionValue))
   {
      directionValue = EnumUtils::toNumber(geometry::Direction::DOWN);
   }

   DEBUG("Adding NPC %s with spritesheet %s", marker.name.c_str(), marker.spritesheet.c_str());
   DEBUG("NPC Location will be (%d, %d)", marker.location.x, marker.location.y);

   marker.direction = static_cast<geometry::Direction>(directionValue);

   auto npc = tileEngine->addNPC(marker);

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
   return scriptEngine->scheduleWork(slideWork, true);
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
