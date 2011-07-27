#include "NPC.h"
#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"
#include "Sprite.h"
#include "Map.h"
#include "TileEngine.h"
#include "Pathfinder.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

const std::string WALKING_PREFIX = "walk_";
const std::string STANDING_PREFIX = "stand_";

NPC::NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name, Spritesheet* sheet, const Map& map,
                       const std::string& regionName,
                       int x, int y) : name(name), map(map), pixelLoc(x, y)
{
   npcThread = engine.getNPCScript(this, regionName, map.getName(), name);
   scheduler.start(npcThread);
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
         Point2D* newCoords = static_cast<Point2D*>(instruction->params);
         MovementDirection newDirection = NONE;

         float vel = 0.1f;
         if(pixelLoc.x < newCoords->x)
         {
            pixelLoc.x += (int)(vel*timePassed);
            pixelLoc.x = pixelLoc.x > newCoords->x ? newCoords->x : pixelLoc.x;
            newDirection = RIGHT;
         }
         else if(pixelLoc.x > newCoords->x)
         {
            pixelLoc.x -= (int)(vel*timePassed);
            pixelLoc.x = pixelLoc.x < newCoords->x ? newCoords->x : pixelLoc.x;
            newDirection = LEFT;
         }

         if(pixelLoc.y < newCoords->y)
         {
            pixelLoc.y += (int)(vel*timePassed);
            pixelLoc.y = pixelLoc.y > newCoords->y ? newCoords->y : pixelLoc.y;
            newDirection = DOWN;
         }
         else if(pixelLoc.y > newCoords->y)
         {
            pixelLoc.y -= (int)(vel*timePassed);
            pixelLoc.y = pixelLoc.y < newCoords->y ? newCoords->y : pixelLoc.y;
            newDirection = UP;
         }

         if(pixelLoc.x == newCoords->x && pixelLoc.y == newCoords->y)
         {
            delete newCoords;
            return true;
         }
         else
         {
            if(newDirection != currDirection)
            {
               currDirection = newDirection;
               switch(currDirection)
               {
                  case LEFT:
                  {
                     sprite->setAnimation(WALKING_PREFIX + "left");
                     break;
                  }
                  case RIGHT:
                  {
                     sprite->setAnimation(WALKING_PREFIX + "right");
                     break;
                  }
                  case UP:
                  {
                     sprite->setAnimation(WALKING_PREFIX + "up");
                     break;
                  }
                  case DOWN:
                  {
                     sprite->setAnimation(WALKING_PREFIX + "down");
                     break;
                  }
                  default:
                  {
                     break;
                  }
               }
            }
            return false;
         }
      }
      case STAND:
      {
         switch(currDirection)
         {
            case LEFT:
            {
               sprite->setFrame(STANDING_PREFIX + "left");
               break;
            }
            case RIGHT:
            {
               sprite->setFrame(STANDING_PREFIX + "right");
               break;
            }
            case UP:
            {
               sprite->setFrame(STANDING_PREFIX + "up");
               break;
            }
            case DOWN:
            {
               sprite->setFrame(STANDING_PREFIX + "down");
               break;
            }
            default:
            {
               break;
            }
         }
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
   sprite->step(timePassed);

   if(!isIdle())
   {
      Instruction* currentInstruction = instructions.front();
      if(runInstruction(currentInstruction, timePassed))
      {
         instructions.pop();
         delete currentInstruction;
      }
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
   Pathfinder* pathfinder = map.getPathfinder();
   
   std::queue<Point2D*> path = pathfinder->findBestPath(pixelLoc.x, pixelLoc.y, x, y);
   while(!path.empty())
   {
      Point2D* coords = path.front();
      path.pop();

      instructions.push(new Instruction(MOVE, coords));
   }

   instructions.push(new Instruction(STAND, NULL));
}

void NPC::setSpritesheet(Spritesheet* sheet)
{
   sprite->setSheet(sheet);
}

void NPC::setFrame(const std::string& frameName)
{
   sprite->setFrame(frameName);
}

void NPC::setAnimation(const std::string& animationName)
{
   sprite->setAnimation(animationName);
}

void NPC::draw()
{
   if(sprite)
   {
      sprite->draw(pixelLoc.x, pixelLoc.y + TileEngine::TILE_SIZE);
   }
}

NPC::~NPC()
{
   delete sprite;
   npcThread->finish();
}
