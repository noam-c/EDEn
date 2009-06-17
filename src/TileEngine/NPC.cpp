#include "NPC.h"
#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

NPC::NPC(ScriptEngine* engine, Scheduler* scheduler, std::string regionName, std::string mapName, std::string name)
{  npcThread = engine->getNPCScript(this, regionName, mapName, name);
   scheduler->start(npcThread);
   DEBUG("NPC %s has a Thread with ID %d", name.c_str(), npcThread->getId());
}

void NPC::step(long timePassed)
{  
}

bool NPC::isIdle()
{  return instructions.empty();
}

void NPC::activate()
{  npcThread->activate();
}

//void NPC::move(int x, int y)
//{  int coords[2] = {x,y};
//   Instruction i = new Instruction(InstructionType::MOVE, coords);
//   instructions.push_back(i);
//}

void NPC::draw()
{
}

NPC::~NPC()
{  npcThread->finish();
}
