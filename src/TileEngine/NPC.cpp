#include "NPC.h"
#include "ScriptEngine.h"
#include "FileScript.h"

NPC::NPC(ScriptEngine* engine, lua_State* luaVM, std::string name)
{  scriptEngine = engine;
//   npcScript = new FileScript(luaVM, name);
}

void NPC::idle()
{  //scriptEngine->callFunction(npcThread, "idle");
}

void NPC::activate()
{  //scriptEngine->callFunction(npcThread, "activate");
}

//void NPC::move(int x, int y)
//{  int coords[2] = {x,y};
//   Instruction i = new Instruction(InstructionType::MOVE, coords);
//   instructions.push_back(i);
//}

void NPC::step()
{  
}

void NPC::draw()
{
}
