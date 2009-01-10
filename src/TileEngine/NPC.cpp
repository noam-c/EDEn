#include "NPC.h"

NPC::NPC(std::string name)
{  scriptEngine = ScriptEngine::getInstance();
   npcThread = scriptEngine->makeThread(name);
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
