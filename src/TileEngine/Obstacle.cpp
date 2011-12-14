/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Obstacle.h"
#include "ResourceLoader.h"
#include "TileEngine.h"
#include "DebugUtils.h"
#include "Sprite.h"

const int DEBUG_FLAG = DEBUG_SPRITE;

Obstacle::Obstacle(const int tileX, const int tileY, const int width, const int height, Spritesheet* spritesheet, const std::string& spriteType, const std::string& spriteName) 
   : tileCoords(Point2D(tileX, tileY)), width(width), height(height)
                                                                                                                                                                                                               {
   sprite = new Sprite(spritesheet);

   if(spriteType == "anim")
   {
      sprite->setAnimation(spriteName);
   }
   else if(spriteType == "frame")
   {
      sprite->setFrame(spriteName);
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
   sprite->draw(tileCoords.x * TileEngine::TILE_SIZE, (tileCoords.y + height) * TileEngine::TILE_SIZE);
}

int Obstacle::getTileX() const
{
   return tileCoords.x;
}

int Obstacle::getTileY() const
{
   return tileCoords.y;
}

int Obstacle::getWidth() const
{
   return width;
}

int Obstacle::getHeight() const
{
   return height;
}
