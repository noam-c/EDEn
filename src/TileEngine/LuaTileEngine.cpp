/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "LuaTileEngine.h"
#include "ScriptUtilities.h"
#include "TileEngine.h"
#include "NPC.h"
#include "Size.h"
#include "Point2D.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_SCRIPT_ENG;

static int TileEngineL_AddNPC(lua_State* luaVM)
{
   NPC* npc = NULL;
   
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == NULL)
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

   DEBUG("Adding NPC %s with spritesheet %s", npcName.c_str(), spritesheetName.c_str());
   DEBUG("NPC Location will be (%d, %d)", x, y);

   const shapes::Point2D npcLocation(x, y);
   const shapes::Size npcSize(width, height);
   npc = tileEngine->addNPC(npcName, spritesheetName, npcLocation, npcSize);
   
   luaW_push<Actor>(luaVM, npc);
   return 1;
}

static int TileEngineL_GetNPC(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == NULL)
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

static int TileEngineL_TilesToPixels(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine == NULL)
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

static luaL_reg tileEngineMetatable[] =
{
   { "addNPC", TileEngineL_AddNPC },
   { "getNPC", TileEngineL_GetNPC },
   { "tilesToPixels", TileEngineL_TilesToPixels },
   { NULL, NULL }
};

void luaopen_TileEngine(lua_State* luaVM)
{
   luaW_register<TileEngine>(luaVM, "TileEngine", NULL, tileEngineMetatable, NULL, NULL);
}
