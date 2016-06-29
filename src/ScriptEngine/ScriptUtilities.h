/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef SCRIPT_UTILITIES_H
#define SCRIPT_UTILITIES_H

#include <string>
#include <vector>

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

#include "LuaWrapper.hpp"

class ScriptUtilities
{
   static bool isBoolean(lua_State* luaStack, int index);
   static bool isTable(lua_State* luaStack, int index);

   static bool isString(lua_State* luaStack, int index);
   static std::string retrieveString(lua_State* luaStack, int index);
   
   static std::vector<std::string> retrieveStringArray(lua_State* luaStack, int index);

   template<typename T> static bool isType(lua_State* luaStack, int index);
   template<typename T> static T* retrieveType(lua_State* luaStack, int index);

   template<typename T> static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, bool (*checkType)(lua_State*, int), T (*retrieve)(lua_State*, int), T& result);

   public:
      template<typename T> static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, T (*retrieve)(lua_State*, int, int*), T& result);
      template<typename T> static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, T*& value);
      static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, int& value);
      static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, long& value);
      static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, float& value);
      static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, double& value);
      static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, bool& value);
      static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, std::string& value);

      static bool getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, std::vector<std::string>& values);
};

template<typename T> bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, bool (*checkType)(lua_State*, int), T (*retrieve)(lua_State*, int), T& result)
{
   bool foundParameter = false;
   if(!parameterName.empty())
   {
      lua_pushstring(luaStack, parameterName.c_str());
      lua_rawget(luaStack, tableIndex);
      if(checkType(luaStack, -1))
      {
         result = retrieve(luaStack, -1);
         foundParameter = true;
      }

      lua_pop(luaStack, 1);
   }

   if(!foundParameter && parameterIndex > 0)
   {
      lua_rawgeti(luaStack, tableIndex, parameterIndex);
      if(checkType(luaStack, -1))
      {
         result = retrieve(luaStack, -1);
         foundParameter = true;
      }

      lua_pop(luaStack, 1);
   }

   return foundParameter;
}

template<typename T> bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, T (*retrieve)(lua_State*, int, int*), T& result)
{
   bool foundParameter = false;
   if(!parameterName.empty())
   {
      lua_pushstring(luaStack, parameterName.c_str());
      lua_rawget(luaStack, tableIndex);

      int success;
      result = retrieve(luaStack, -1, &success);
      foundParameter = (success == 1);

      lua_pop(luaStack, 1);
   }

   if(!foundParameter && parameterIndex > 0)
   {
      lua_rawgeti(luaStack, tableIndex, parameterIndex);

      int success;
      result = retrieve(luaStack, -1, &success);
      foundParameter = (success == 1);

      lua_pop(luaStack, 1);
   }

   return foundParameter;
}

template<typename T> bool ScriptUtilities::isType(lua_State* luaStack, int index)
{
   return luaW_is<T>(luaStack, index);
}

template<typename T> T* ScriptUtilities::retrieveType(lua_State* luaStack, int index)
{
   return luaW_to<T>(luaStack, index);
}

template<typename T> bool ScriptUtilities::getParameter(lua_State* luaStack, int tableIndex, int parameterIndex, const std::string parameterName, T*& value)
{
   return ScriptUtilities::getParameter(luaStack, tableIndex, parameterIndex, parameterName, ScriptUtilities::isType<T>, ScriptUtilities::retrieveType<T>, value);
}

#endif
