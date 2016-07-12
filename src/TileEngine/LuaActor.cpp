/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "LuaActor.h"
#include "ScriptUtilities.h"
#include "ScriptEngine.h"
#include "Actor.h"

static int ActorL_Move(lua_State* luaVM)
{
   Actor* actor = luaW_check<Actor>(luaVM, 1);
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

static int ActorL_SetSprite(lua_State* luaVM)
{
   Actor* actor = luaW_check<Actor>(luaVM, 1);
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

static int ActorL_SetAnimation(lua_State* luaVM)
{
   Actor* actor = luaW_check<Actor>(luaVM, 1);
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

static int ActorL_SetSpritesheet(lua_State* luaVM)
{
   Actor* actor = luaW_check<Actor>(luaVM, 1);
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

static int ActorL_LookAt(lua_State* luaVM)
{
   Actor* actor = luaW_check<Actor>(luaVM, 1);
   if (actor == nullptr)
   {
      return lua_error(luaVM);
   }

   Actor* target;
   if(!ScriptUtilities::getParameter<Actor>(luaVM, 2, 1, "target", target))
   {
      return lua_error(luaVM);
   }

   actor->faceActor(target);

   return 0;
}

static luaL_Reg actorMetatable[] =
{
   { "move", ActorL_Move },
   { "setSprite", ActorL_SetSprite },
   { "setAnimation", ActorL_SetAnimation },
   { "setSpritesheet", ActorL_SetSpritesheet },
   { "lookAt", ActorL_LookAt },
   { nullptr, nullptr }
};

void luaopen_Actor(lua_State* luaVM)
{
   luaW_register<Actor>(luaVM, "Actor", nullptr, actorMetatable, nullptr, nullptr);
}
