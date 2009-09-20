#include "NPC.h"
#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"
#include "Sprite.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

NPC::NPC(ScriptEngine* engine, Scheduler* scheduler, Spritesheet* sheet,
                       std::string regionName, std::string mapName, std::string name,
                       int x, int y) : name(name), x(x), y(y)
{
   npcThread = engine->getNPCScript(this, regionName, mapName, name);
   scheduler->start(npcThread);
   DEBUG("NPC %s has a Thread with ID %d", name.c_str(), npcThread->getId());

   sprite = new Sprite(sheet);
}

std::string NPC::getName()
{
   return name;
}

bool NPC::runInstruction(Instruction* instruction, long timePassed)
{
   switch(instruction->type)
   {
      case MOVE:
      {
         int* newCoords = static_cast<int*>(instruction->params);
         x = newCoords[0];
         y = newCoords[1];

         DEBUG("%s is now at %d, %d", name.c_str(), x, y);
         delete newCoords;

         return true;
      }
      default:
      {
         // Unknown instruction type; mark it finished and move on
         return true;
      }
   }
}

void NPC::step(long timePassed)
{
   if(isIdle()) return;

   Instruction* currentInstruction = instructions.front();
   if(runInstruction(currentInstruction, timePassed))
   {
      instructions.pop();
      delete currentInstruction;
   }
}

bool NPC::isIdle()
{
   return instructions.empty();
}

void NPC::activate()
{
   npcThread->activate();
}

void NPC::move(int x, int y)
{
   int* coords = new int[2];
   coords[0] = x;
   coords[1] = y;

   Instruction* i = new Instruction(MOVE, coords);
   instructions.push(i);
}

void NPC::draw()
{
   if(sprite)
   {
      sprite->draw(x, y);
   }
}

NPC::~NPC()
{
   delete sprite;
   npcThread->finish();
}
