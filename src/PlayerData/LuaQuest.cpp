/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "LuaQuest.h"
#include "Quest.h"

#include "ScriptUtilities.h"
#include "DebugUtils.h"
const int debugFlag = DEBUG_SCRIPT_ENG;

static int QuestL_AddQuest(lua_State* luaVM)
{
   Quest* parentQuest = luaW_check<Quest>(luaVM, 1);
   if(parentQuest == NULL)
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

   Quest* newQuest = new Quest(name, description, completed, optional);
   parentQuest->addQuest(newQuest);
   luaW_push<Quest>(luaVM, newQuest);
   return 1;
}

static int QuestL_GetQuest(lua_State* luaVM)
{
   Quest* parentQuest = luaW_check<Quest>(luaVM, 1);
   if(parentQuest == NULL)
   {
      return lua_error(luaVM);
   }

   std::string questPath;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "path", questPath))
   {
      return lua_error(luaVM);
   }

   Quest* questToGet = parentQuest->getQuest(questPath);
   luaW_push<Quest>(luaVM, questToGet);
   return 1;
}

static int QuestL_IsStarted(lua_State* luaVM)
{
   Quest* quest = luaW_to<Quest>(luaVM, 1);
   lua_pushboolean(luaVM, quest != NULL);
   return 1;
}

static int QuestL_IsComplete(lua_State* luaVM)
{
   Quest* quest = luaW_check<Quest>(luaVM, 1);
   if(quest == NULL)
   {
      return lua_error(luaVM);
   }

   lua_pushboolean(luaVM, quest->isCompleted());
   return 1;
}

static int QuestL_Complete(lua_State* luaVM)
{
   Quest* quest = luaW_check<Quest>(luaVM, 1);
   if(quest == NULL)
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
   { NULL, NULL }
};

void luaopen_Quest(lua_State* luaVM)
{
   luaW_register<Quest>(luaVM, "Quest", NULL, questMetatable, NULL, NULL);
}
