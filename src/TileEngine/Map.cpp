/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Map.h"
#include "Tileset.h"
#include "Obstacle.h"
#include "Pathfinder.h"
#include "ResourceLoader.h"
#include "TileEngine.h"
#include "DebugUtils.h"

#include <sstream>

const int debugFlag = DEBUG_RES_LOAD;

//#define DRAW_PASSIBILITY

Map::Map()
{
}

Map::Map(std::ifstream& in)
{
   /**
    * \todo Regions shouldn't assume that files are well-formed.
    *       Find and report errors if the file isn't formed ideally.
    */

   std::getline(in, mapName, MAP_DELIM);
   DEBUG("Loading map: %s", mapName.c_str());
   std::getline(in, tilesetName, MAP_DELIM);

   in >> width;
   in.get();
   in >> height;

   tileset = ResourceLoader::getTileset(tilesetName);

   tileMap = new int*[height];

   for(int i = 0; i < height; ++i)
   {
      tileMap[i] = new int[width];
      for(int j = 0; j < width; ++j)
      {
         if(in)
         {
            in >> tileMap[i][j];
         }
         else
         {
            T_T("Tile map incomplete.");
         }
      }

      in.ignore(width<<1, '\n');
   }

   std::string obstacleLine;
   std::string obstacleSheetName;
   std::string obstacleSpriteType;
   std::string obstacleSpriteName;
   
   int obstacleWidth;
   int obstacleHeight;
   int tileX;
   int tileY;

   for(;;)
   {
      if(!std::getline(in, obstacleLine) || obstacleLine == "\n") break;

      std::istringstream lineStream(obstacleLine);
      lineStream >> obstacleSheetName >> obstacleSpriteType >> obstacleSpriteName >> obstacleWidth >> obstacleHeight >> tileX >> tileY;

      Spritesheet* obstacleSheet = ResourceLoader::getSpritesheet(obstacleSheetName);
      obstacles.push_back(new Obstacle(tileX, tileY, obstacleWidth, obstacleHeight, obstacleSheet, obstacleSpriteType, obstacleSpriteName));
   }

   initializePassibilityMatrix();
   pathfinder = new Pathfinder(this, obstacles);

   DEBUG("Map loaded.");
}

bool Map::isPassible(int x, int y) const
{
   return tileset->isPassible(tileMap[y][x]);
}

void Map::initializePassibilityMatrix()
{
   passibilityMap = new bool*[height];
   for(int y = 0; y < height; ++y)
   {
      passibilityMap[y] = new bool[width];
      for(int x = 0; x < width; ++x)
      {
         passibilityMap[y][x] = isPassible(x, y);
      }
   }
}

std::string Map::getName() const
{
   return mapName;
}

int Map::getWidth() const
{
   return width;
}

int Map::getHeight() const
{
   return height;
}

Pathfinder* Map::getPathfinder() const
{
   return pathfinder;
}

bool** Map::getPassibilityMatrix() const
{
   return passibilityMap;
}

void Map::step(long timePassed)
{
   std::vector<Obstacle*>::iterator iter;
   for(iter = obstacles.begin(); iter != obstacles.end(); ++iter)
   {
      (*iter)->step(timePassed);
   }
}

void Map::draw() const
{
   for(int i = 0; i < width; ++i)
   {
      for(int j = 0; j < height; ++j)
      {
#ifdef DRAW_PASSIBILITY
         if(passibilityMap[j][i])
         {
            Tileset::drawColorToTile(i, j, 0.0f, 1.0f, 0.0f);
         }
         else
         {
            Tileset::drawColorToTile(i, j, 1.0f, 0.0f, 0.0f);
         }
#else
         tileset->draw(i, j, tileMap[j][i]);
#endif
      }
   }

   std::vector<Obstacle*>::const_iterator iter;
   for(iter = obstacles.begin(); iter != obstacles.end(); ++iter)
   {
      (*iter)->draw();
   }
}

Map::~Map()
{
   for(int i = 0; i < width; ++i)
   {
      delete [] tileMap[i];
      delete [] passibilityMap[i];
   }

   for (unsigned int i = 0; i < obstacles.size(); i++)
   {
      delete obstacles[i];
   }

   delete [] tileMap;
   delete [] passibilityMap;

   delete pathfinder;
}
