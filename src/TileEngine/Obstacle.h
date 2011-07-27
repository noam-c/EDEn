#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include <string>
#include <iostream>
#include "ResourceKey.h"
#include "Point2D.h"

class Sprite;
class Spritesheet;

class Obstacle
{
   const Point2D tileCoords;

   const int width;
   const int height;

   Sprite* sprite;

   public:
     Obstacle(const int tileX, const int tileY, const int width, const int height, Spritesheet* spritesheet, const std::string& spriteType, const std::string& spriteName);

     void step(long timePassed) const;
     void draw() const;
     int getTileX() const;
     int getTileY() const;
	  int getWidth() const;
	  int getHeight() const;
};

#endif