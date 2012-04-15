/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "LuaActor.h"
#include "Actor.h"
#include "Point2D.h"

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int ActorL_Move(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 3:
      {
         Actor* actor = luaW_check<Actor>(luaVM, 1);
         if (actor)
         {
            const shapes::Point2D destination(lua_tointeger(luaVM, 2), lua_tointeger(luaVM, 3));
            actor->move(destination);
         }
         break;
      }
   }
   return 0;
}

static int ActorL_SetSprite(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 2:
      {
         Actor* actor = luaW_check<Actor>(luaVM, 1);
         if (actor)
         {
            std::string frameName(lua_tostring(luaVM, 2));
            actor->setFrame(frameName);
         }
         break;
      }
   }
   
   return 0;
}

static int ActorL_SetAnimation(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 2:
      {
         Actor* actor = luaW_check<Actor>(luaVM, 1);
         if (actor)
         {
            std::string animationName(lua_tostring(luaVM, 2));
            actor->setAnimation(animationName);
         }
         break;
      }
   }
   
   return 0;
}

static int ActorL_SetSpritesheet(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 2:
      {
         Actor* actor = luaW_check<Actor>(luaVM, 1);
         if (actor)
         {
            std::string spritesheetName(lua_tostring(luaVM, 2));
            actor->setSpritesheet(spritesheetName);
         }
         break;
      }
   }
   
   return 0;
}

static int ActorL_LookAt(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 2:
      {
         Actor* actor = luaW_check<Actor>(luaVM, 1);
         Actor* other = luaW_check<Actor>(luaVM, 2);
         
         actor->faceActor(other);
         break;
      }
   }

   return 0;
}

static luaL_reg actorMetatable[] =
{
   { "move", ActorL_Move },
   { "setSprite", ActorL_SetSprite },
   { "setAnimation", ActorL_SetAnimation },
   { "setSpritesheet", ActorL_SetSpritesheet },
   { "lookAt", ActorL_LookAt },
   { NULL, NULL }
};

void luaopen_Actor(lua_State* luaVM)
{
   luaW_register<Actor>(luaVM, "Actor", NULL, actorMetatable, NULL, NULL);
}
