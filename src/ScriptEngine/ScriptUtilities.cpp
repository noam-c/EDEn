/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ScriptUtilities.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_SCRIPT_ENG;

bool ScriptUtilities::isBoolean(lua_State* luaStack, int index)
{
   return lua_isboolean(luaStack, index);
}

bool ScriptUtilities::isString(lua_State* luaStack, int index)
{
   return lua_type(luaStack, index) == LUA_TSTRING;
}

std::string ScriptUtilities::retrieveString(lua_State* luaStack, int index)
{
   return std::string(lua_tolstring(luaStack, index, nullptr));
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
