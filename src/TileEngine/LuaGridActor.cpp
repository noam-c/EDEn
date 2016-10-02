/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "LuaGridActor.h"
#include "ScriptUtilities.h"
#include "ScriptEngine.h"
#include "GridActor.h"

static int GridActorL_Move(lua_State* luaVM)
{
   GridActor* actor = luaW_check<GridActor>(luaVM, 1);
   if (actor == nullptr)
   {
      return lua_error(luaVM);
   }

   int x;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "x", x))
   {
      return lua_error(luaVM);
   }

   int y;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "y", y))
   {
      return lua_error(luaVM);
   }

   bool waitForFinish;
   if(!ScriptUtilities::getParameter(luaVM, 2, 3, "waitForFinish", waitForFinish))
   {
      waitForFinish = false;
   }

   auto moveWork = [actor, x, y](const std::shared_ptr<Task>& task)
   {
      actor->move({x, y}, task);
   };
   
   auto scriptEngine = ScriptEngine::getScriptEngineForVM(luaVM);
   return scriptEngine->scheduleWork(moveWork, waitForFinish);
}

static int GridActorL_SetSprite(lua_State* luaVM)
{
   GridActor* actor = luaW_check<GridActor>(luaVM, 1);
   if (actor == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string frameName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "frame", frameName))
   {
      return lua_error(luaVM);
   }

   return 0;
}

static int GridActorL_SetAnimation(lua_State* luaVM)
{
   GridActor* actor = luaW_check<GridActor>(luaVM, 1);
   if (actor == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string animationName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "animation", animationName))
   {
      return lua_error(luaVM);
   }

   actor->setAnimation(animationName);

   return 0;
}

static int GridActorL_SetSpritesheet(lua_State* luaVM)
{
   GridActor* actor = luaW_check<GridActor>(luaVM, 1);
   if (actor == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string spritesheetName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "spritesheet", spritesheetName))
   {
      return lua_error(luaVM);
   }

   actor->setSpritesheet(spritesheetName);

   return 0;
}

static int GridActorL_LookAt(lua_State* luaVM)
{
   GridActor* actor = luaW_check<GridActor>(luaVM, 1);
   if (actor == nullptr)
   {
      return lua_error(luaVM);
   }

   GridActor* target;
   if(!ScriptUtilities::getParameter<GridActor>(luaVM, 2, 1, "target", target))
   {
      return lua_error(luaVM);
   }

   actor->faceActor(target);

   return 0;
}

static luaL_Reg gridActorMetatable[] =
{
   { "move", GridActorL_Move },
   { "setSprite", GridActorL_SetSprite },
   { "setAnimation", GridActorL_SetAnimation },
   { "setSpritesheet", GridActorL_SetSpritesheet },
   { "lookAt", GridActorL_LookAt },
   { nullptr, nullptr }
};

void luaopen_GridActor(lua_State* luaVM)
{
   luaW_register<GridActor>(luaVM, "GridActor", nullptr, gridActorMetatable, nullptr, nullptr);
}
