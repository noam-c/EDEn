/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "LuaTileEngine.h"
#include "ScriptUtilities.h"
#include "TileEngine.h"
#include "NPC.h"
#include "Size.h"
#include "Point2D.h"
#include "MapTriggerCallback.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_SCRIPT_ENG;

static int TileEngineL_AddNPC(lua_State* luaVM)
{
   NPC* npc = nullptr;

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

   int width;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "width", width))
   {
      width = 32;
   }

   int height;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "height", height))
   {
      height = 32;
   }

   int directionValue;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "direction", directionValue))
   {
      directionValue = static_cast<int>(DOWN);
   }

   DEBUG("Adding NPC %s with spritesheet %s", npcName.c_str(), spritesheetName.c_str());
   DEBUG("NPC Location will be (%d, %d)", x, y);

   const shapes::Point2D npcLocation(x, y);
   const shapes::Size npcSize(width, height);
   const MovementDirection direction = static_cast<MovementDirection>(directionValue);

   npc = tileEngine->addNPC(npcName, spritesheetName, npcLocation, npcSize, direction);

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

   const shapes::Point2D destination(destinationX, destinationY);

   int originX;
   int originY;
   bool originXProvided = ScriptUtilities::getParameter(luaVM, 2, 3, "originX", originX);
   bool originYProvided = ScriptUtilities::getParameter(luaVM, 2, 4, "originY", originY);

   shapes::Point2D origin;
   if(originXProvided && originYProvided)
   {
      origin.x = originX;
      origin.y = originY;
   }
   else if(!originXProvided && !originYProvided)
   {
      origin = tileEngine->getCurrentCameraLocation();
   }
   else
   {
      return lua_error(luaVM);
   }

   double speed;
   if(!ScriptUtilities::getParameter(luaVM, 2, 5, "speed", speed) || speed <= 0.0f)
   {
      speed = 0.1f;
   }

   return tileEngine->slideCamera(origin, destination, speed);
}

static int TileEngineL_OpenSaveMenu(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == nullptr)
   {
      return lua_error(luaVM);
   }

   return tileEngine->openSaveMenu();
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
