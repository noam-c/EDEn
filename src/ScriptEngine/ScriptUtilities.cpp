/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ScriptUtilities.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_SCRIPT_ENG

bool ScriptUtilities::isBoolean(lua_State* luaStack, int index)
{
   return lua_isboolean(luaStack, index);
}

bool ScriptUtilities::isTable(lua_State* luaStack, int index)
{
   return lua_istable(luaStack, index);
}

bool ScriptUtilities::isString(lua_State* luaStack, int index)
{
   return lua_type(luaStack, index) == LUA_TSTRING;
}

std::string ScriptUtilities::retrieveString(lua_State* luaStack, int index)
{
   return std::string(lua_tolstring(luaStack, index, nullptr));
}

std::vector<std::string> ScriptUtilities::retrieveStringArray(lua_State* luaStack, int index)
{
   std::vector<std::string> results;

   // Traverse the string array now at the top of the stack
   // This traversal code is modified from the Lua documentation's table traversal
   // to only traverse the longest valid array-like sequence in a table, from 0.
   lua_pushnil(luaStack);  // push the first key
   while (lua_next(luaStack, index - 1) != 0) {
      // 'key' is at index -2, and 'value' is at index -1
      if(!lua_isnumber(luaStack, -2))
      {
         // We only care about the numeric sequence
         break;
      }

      if (ScriptUtilities::isString(luaStack, -1))
      {
         results.emplace_back(ScriptUtilities::retrieveString(luaStack, -1));
      }

      // remove 'value'; keep 'key' at index -1 for the next iteration
      lua_pop(luaStack, 1);
   }

   return results;
}

bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, int& value)
{
   lua_Integer luaIntegerResult;
   bool foundParameter = ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, lua_tointegerx, luaIntegerResult);

   if (foundParameter)
   {
      value = static_cast<int>(luaIntegerResult);
      return true;
   }

   return false;
}

bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, long& value)
{
   lua_Integer luaIntegerResult;
   bool foundParameter = ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, lua_tointegerx, luaIntegerResult);

   if (foundParameter)
   {
      value = static_cast<long>(luaIntegerResult);
      return true;
   }

   return false;
}

bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, float& value)
{
   lua_Number luaNumericResult;
   bool foundParameter = ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, lua_tonumberx, luaNumericResult);

   if (foundParameter)
   {
      value = static_cast<float>(luaNumericResult);
      return true;
   }

   return false;
}

bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, double& value)
{
   lua_Number luaNumericResult;
   bool foundParameter = ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, lua_tonumberx, luaNumericResult);

   if (foundParameter)
   {
      value = static_cast<double>(luaNumericResult);
      return true;
   }

   return false;
}

bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, bool& value)
{
   int booleanValue;
   bool foundParameter = ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, ScriptUtilities::isBoolean, lua_toboolean, booleanValue);

   if (foundParameter)
   {
      value = booleanValue != 0;
      return true;
   }

   return false;
}

bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, std::string& value)
{
   return ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, ScriptUtilities::isString, ScriptUtilities::retrieveString, value);
}

bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, std::vector<std::string>& values)
{
   return ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, ScriptUtilities::isTable, ScriptUtilities::retrieveStringArray, values);
}

