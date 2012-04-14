/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Obstacle.h"
#include "ResourceLoader.h"
#include "TileEngine.h"
#include "DebugUtils.h"
#include "Sprite.h"

const int DEBUG_FLAG = DEBUG_SPRITE;

Obstacle::Obstacle(const shapes::Point2D& tileCoords, const shapes::Size& size, Spritesheet* spritesheet, const std::string& spriteType, const std::string& spriteName) 
   : tileCoords(tileCoords), size(size)
{
   sprite = new Sprite(spritesheet);

   if(spriteType == "anim")
   {
      sprite->setAnimation(spriteName, NONE);
   }
   else if(spriteType == "frame")
   {
      sprite->setFrame(spriteName, NONE);
   }
   else
   {
      T_T("Undefined obstacle sprite type found in map. Only 'frame' or 'anim' are suitable.");
   }
}

void Obstacle::step(long timePassed) const
{
   sprite->step(timePassed);
}

void Obstacle::draw() const
{
   sprite->draw(tileCoords.x * TileEngine::TILE_SIZE, (tileCoords.y + size.height) * TileEngine::TILE_SIZE);
}

const shapes::Point2D& Obstacle::getTileCoords() const
{
   return tileCoords;
}

const shapes::Size& Obstacle::getSize() const
{
   return size;
}
