#include "LuaQuest.h"
#include "Quest.h"

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int QuestL_AddQuest(lua_State* luaVM)
{
   std::string name;
   std::string description;
   bool optional = false;
   bool completed = false;
   Quest* newQuest = NULL;

   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 5:
      {
         completed = lua_toboolean(luaVM, 5);
      }
      case 4:
      {
         optional = lua_toboolean(luaVM, 4);
      }
      case 3:
      {
         description = lua_tostring(luaVM, 3);
      }
      case 2:
      {
         name = lua_tostring(luaVM, 2);

         Quest* parentQuest = luaW_check<Quest>(luaVM, 1);
         if(parentQuest != NULL)
         {
            newQuest = new Quest(name, description, completed, optional);
            parentQuest->addQuest(newQuest);
         }
         break;
      }
   }

   luaW_push<Quest>(luaVM, newQuest);
   return 1;
}

static int QuestL_GetQuest(lua_State* luaVM)
{
   Quest* questToGet = NULL;
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 2:
      {
         Quest* parent = luaW_check<Quest>(luaVM, 1);
         if(parent != NULL)
         {
            const std::string questPath = lua_tostring(luaVM, 2);
            questToGet = parent->getQuest(questPath);
         }
         break;
      }
   }
   luaW_push<Quest>(luaVM, questToGet);
   return 1;
}

static int QuestL_IsStarted(lua_State* luaVM)
{
   bool started = false;
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 1:
      {
         Quest* quest = luaW_check<Quest>(luaVM, 1);
         if(quest != NULL)
         {
            started = true;
         }
         break;
      }
   }
   
   lua_pushboolean(luaVM, started);
   return 1;
}

static int QuestL_IsComplete(lua_State* luaVM)
{
   bool complete = false;
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 1:
      {
         Quest* quest = luaW_check<Quest>(luaVM, 1);
         if(quest != NULL)
         {
            complete = quest->isCompleted();
         }
         break;
      }
   }
   
   lua_pushboolean(luaVM, complete);
   return 1;
}

static int QuestL_Complete(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);

   switch(nargs)
   {
      case 1:
      {
         Quest* quest = luaW_check<Quest>(luaVM, 1);
         if(quest != NULL)
         {
            quest->complete();
         }
         break;
      }
   }
   
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
