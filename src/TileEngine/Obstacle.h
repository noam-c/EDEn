/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <string>
#include <iostream>
#include "ResourceKey.h"
#include "Size.h"
#include "Point2D.h"

class Sprite;
class Spritesheet;

class Obstacle
{
   const shapes::Point2D tileCoords;
   const shapes::Size size;

   Sprite* sprite;

   public:
      Obstacle(const shapes::Point2D& tileCoords, const shapes::Size& size, Spritesheet* spritesheet, const std::string& spriteType, const std::string& spriteName);

      void step(long timePassed) const;
      void draw() const;
      const shapes::Point2D& getTileCoords() const;
      const shapes::Size& getSize() const;
};

#endif
