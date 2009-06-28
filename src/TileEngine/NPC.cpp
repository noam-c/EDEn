#include "NPC.h"
#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"
#include "Sprite.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

NPC::NPC(ScriptEngine* engine, Scheduler* scheduler, Spritesheet* sheet,
                       std::string regionName, std::string mapName, std::string name)
{  npcThread = engine->getNPCScript(this, regionName, mapName, name);
   scheduler->start(npcThread);
   DEBUG("NPC %s has a Thread with ID %d", name.c_str(), npcThread->getId());

   sprite = new Sprite(sheet);
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
{  if(sprite)
   {  sprite->draw(x, y);
   }
}

NPC::~NPC()
{  delete sprite;
   npcThread->finish();
}
