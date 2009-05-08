#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Thread.h"
#include <string>

struct lua_State;

class Script : public Thread
{  lua_State* luaStack;

   bool runScript();

   public:
      Script(lua_State* luaVM, std::string scriptPath);
      bool resume(long timePassed);
      int yield();
};

#endif
