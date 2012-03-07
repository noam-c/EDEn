#include "Actor.h"
#include "ResourceLoader.h"
#include "EntityGrid.h"
#include "Sprite.h"
#include "TileEngine.h"
#include "Actor_Orders.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

Actor::Actor(const std::string& name, const std::string& sheetName, EntityGrid& entityGrid, int x, int y, double movementSpeed, MovementDirection direction)
   : name(name), width(32), height(32), pixelLoc(x, y), movementSpeed(movementSpeed), currDirection(direction), entityGrid(entityGrid)
{
   Spritesheet* sheet = ResourceLoader::getSpritesheet(sheetName);
   sprite = new Sprite(sheet);
}

Actor::~Actor()
{
   flushOrders();
   delete sprite;
}

void Actor::flushOrders()
{
   while(!orders.empty())
   {
      Order* order = orders.front();
      orders.pop();
      delete order;
   }
}

std::string Actor::getName() const
{
   return name;
}

int Actor::getWidth() const
{
   return width;
}

int Actor::getHeight() const
{
   return height;
}

void Actor::step(long timePassed)
{
   sprite->step(timePassed);
   
   if(!isIdle())
   {
      Order* currentOrder = orders.front();
      if(currentOrder->perform(timePassed))
      {
         orders.pop();
         delete currentOrder;
      }
   }
}

void Actor::draw()
{
   if(sprite)
   {
      sprite->draw(pixelLoc.x, pixelLoc.y + TileEngine::TILE_SIZE);
   }
   
   if(!orders.empty())
   {
      orders.front()->draw();
   }
}

bool Actor::isIdle() const
{
   return orders.empty();
}

void Actor::move(int x, int y)
{
   if(entityGrid.withinMap(x,y))
   {
      DEBUG("Sending move order to %s: %d,%d", name.c_str(), x, y);
      shapes::Point2D dst(x, y);
      orders.push(new MoveOrder(*this, dst, entityGrid));
   }
   else
   {
      DEBUG("%d,%d is not a place!!!", x, y);
   }
}

void Actor::stand(MovementDirection direction)
{
   orders.push(new StandOrder(*this, direction));
}

void Actor::faceActor(Actor* other)
{
   shapes::Point2D currentLocation = getLocation();
   shapes::Point2D otherLocation = other->getLocation();
   MovementDirection directionToOther = getDirection();

   int xDiff = currentLocation.x - otherLocation.x;
   int yDiff = currentLocation.y - otherLocation.y;
   
   if(abs(xDiff) > abs(yDiff))
   {
      if(xDiff < 0)
      {
         directionToOther = RIGHT;
      }
      else if(xDiff > 0)
      {
         directionToOther = LEFT;
      }  
   }
   else
   {
      if(yDiff < 0)
      {
         directionToOther = DOWN;
      }
      else if(yDiff > 0)
      {
         directionToOther = UP;
      }
   }
   
   stand(directionToOther);
}

void Actor::setSpritesheet(const std::string& sheetName)
{
   Spritesheet* sheet = ResourceLoader::getSpritesheet(sheetName);
   sprite->setSheet(sheet);
}

void Actor::setFrame(const std::string& frameName)
{
   sprite->setFrame(frameName, currDirection);
}

void Actor::setAnimation(const std::string& animationName)
{
   sprite->setAnimation(animationName, currDirection);
}

void Actor::setLocation(shapes::Point2D location)
{
   pixelLoc = location;
}

shapes::Point2D Actor::getLocation() const
{
   return pixelLoc;
}

void Actor::setDirection(MovementDirection direction)
{
   currDirection = direction;
}

MovementDirection Actor::getDirection() const
{
   return currDirection;
}

void Actor::setMovementSpeed(float speed)
{
   movementSpeed = speed;
}

float Actor::getMovementSpeed() const
{
   return movementSpeed;
}