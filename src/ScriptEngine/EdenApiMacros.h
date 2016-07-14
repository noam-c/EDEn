/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ScriptUtilities.h"

#define EDEN_API_CLASS(className, metatable) \
void luaopen_ ## className(lua_State* luaVM) \
{ \
   luaW_register<className>(luaVM, #className, nullptr, metatable, nullptr, nullptr); \
}

#define EDEN_API_START_FUNC(cName) \
static int cName(lua_State* luaVM) \
{ \
  unsigned int paramIndex = 1; \
  unsigned int numResults = 0;

#define EDEN_API_END_FUNC() \
   return numResults; \
} \

#define EDEN_API_CONTEXT(Type, contextName) \
  Type* contextName = luaW_check<Type>(luaVM, 1); \
  if(contextName == nullptr) \
  { \
    return lua_error(luaVM); \
  }

#define EDEN_API_NO_PARAMS() \
  (void)paramIndex;

#define EDEN_API_PARAM(Type, paramName) \
   Type paramName; \
   if(!ScriptUtilities::getParameter(luaVM, 2, paramIndex, #paramName, paramName)) \
   { \
      return lua_error(luaVM); \
   } \
   ++paramIndex;

#define EDEN_API_OPT_PARAM(Type, paramName, defaultValue) \
   Type paramName; \
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, #paramName, paramName)) \
   { \
      paramName = defaultValue; \
   }

#define EDEN_API_RETURN(value) \
   ScriptUtilities::pushParameter(luaVM, value); \
   ++numResults;
