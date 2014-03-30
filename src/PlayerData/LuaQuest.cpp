/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "LuaQuest.h"
#include "Quest.h"

#include "ScriptUtilities.h"
#include "DebugUtils.h"
const int debugFlag = DEBUG_SCRIPT_ENG;

static int QuestL_AddQuest(lua_State* luaVM)
{
   Quest* parentQuest = luaW_check<Quest>(luaVM, 1);
   if(parentQuest == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string name;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "name", name))
   {
      return lua_error(luaVM);
   }

   std::string description;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "description", description))
   {
      description = "";
   }

   bool optional;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "optional", optional))
   {
      optional = false;
   }

   bool completed;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "completed", completed))
   {
      completed = false;
   }

   auto newQuest = std::make_shared<Quest>(name, description, completed, optional);
   parentQuest->addQuest(newQuest);
   luaW_push<Quest>(luaVM, newQuest.get());
   return 1;
}

static int QuestL_GetQuest(lua_State* luaVM)
{
   Quest* parentQuest = luaW_check<Quest>(luaVM, 1);
   if(parentQuest == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string questPath;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "path", questPath))
   {
      return lua_error(luaVM);
   }

   auto questToGet = parentQuest->getQuest(questPath).lock();
   if(questToGet)
   {
      luaW_push<Quest>(luaVM, questToGet.get());
   }
   else
   {
      lua_pushnil(luaVM);
   }

   return 1;
}

static int QuestL_IsStarted(lua_State* luaVM)
{
   Quest* parentQuest = luaW_check<Quest>(luaVM, 1);
   if(parentQuest == nullptr)
   {
      return lua_error(luaVM);
   }
   
   std::string questPath;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "path", questPath))
   {
      questPath = "";
   }
   
   Quest* quest = luaW_to<Quest>(luaVM, 1);
   if(questPath.empty())
   {
      lua_pushboolean(luaVM, quest != nullptr);
   }
   else
   {
      lua_pushboolean(luaVM, quest != nullptr && quest->isStarted(questPath));
   }

   return 1;
}

static int QuestL_IsComplete(lua_State* luaVM)
{
   Quest* parentQuest = luaW_check<Quest>(luaVM, 1);
   if(parentQuest == nullptr)
   {
      return lua_error(luaVM);
   }
   
   std::string questPath;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "path", questPath))
   {
      questPath = "";
   }
   
   Quest* quest = luaW_to<Quest>(luaVM, 1);
   if(questPath.empty())
   {
      lua_pushboolean(luaVM, quest != nullptr && quest->isCompleted());
   }
   else
   {
      lua_pushboolean(luaVM, quest != nullptr && quest->isCompleted(questPath));
   }

   return 1;
}

static int QuestL_Complete(lua_State* luaVM)
{
   Quest* quest = luaW_check<Quest>(luaVM, 1);
   if(quest == nullptr)
   {
      return lua_error(luaVM);
   }

   quest->complete();
   return 0;
}

static luaL_reg questMetatable[] =
{
   { "add", QuestL_AddQuest },
   { "get", QuestL_GetQuest },
   { "isStarted", QuestL_IsStarted },
   { "isComplete", QuestL_IsComplete },
   { "complete", QuestL_Complete },
   { nullptr, nullptr }
};

void luaopen_Quest(lua_State* luaVM)
{
   luaW_register<Quest>(luaVM, "Quest", nullptr, questMetatable, nullptr, nullptr);
}
