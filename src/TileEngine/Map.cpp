/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Map.h"
#include "Tileset.h"
#include "Obstacle.h"
#include "Pathfinder.h"
#include "ResourceLoader.h"
#include "TileEngine.h"
#include "DebugUtils.h"
#include "tinyxml.h"

#include <sstream>

const int debugFlag = DEBUG_RES_LOAD;

//#define DRAW_PASSIBILITY

Map::Map()
{
}

Map::Map(const std::string& name, const std::string& filePath) : mapName(name)
{
   DEBUG("Loading map file %s", filePath.c_str());
   
   std::ifstream input(filePath.c_str());
   if(!input)
   {
      T_T("Failed to open map file for reading.");
   }
   
   TiXmlDocument xmlDoc;
   input >> xmlDoc;
   
   if(xmlDoc.Error())
   {
      DEBUG("Error occurred in map XML parsing: %s", xmlDoc.ErrorDesc());
      T_T("Failed to parse map data.");
   }
   
   TiXmlElement* root = xmlDoc.RootElement();
   if(strcmp(root->Value(), "map") != 0)
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse map data.");
   }
   
   root->Attribute("width", &width);
   root->Attribute("height", &height);
   
   TiXmlElement* propertiesElement = root->FirstChildElement("properties");
   
   TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
   while(propertyElement != NULL)
   {
      if(std::string(propertyElement->Attribute("name")) == "tilesetName")
      {
         std::string tilesetName = propertyElement->Attribute("value");
         tileset = ResourceLoader::getTileset(tilesetName);
         break;
      }
      
      propertyElement = propertyElement->NextSiblingElement("property");
   }
   
   if(tileset == NULL)
   {
      T_T("Map doesn't contain a tileset.");
   }
   
   TiXmlElement* floorElement = root->FirstChildElement("layer");
   if(floorElement == NULL)
   {
      DEBUG("Expected layer data in map.");
      T_T("Failed to parse map data.");
   }
   
   TiXmlText* floorDataElement = floorElement->FirstChildElement("data")->FirstChild()->ToText();
   std::stringstream layerStream(floorDataElement->Value());
   
   tileMap = new int*[height];
   for(int y = 0; y < height; ++y)
   {
      tileMap[y] = new int[width];
      
      for(int x = 0; x < width; ++x)
      {
         std::string entry;
         std::getline(layerStream, entry, ',');
         tileMap[y][x] = atoi(entry.c_str()) - 1;
      }
   }
   
   initializePassibilityMatrix();
   
   std::vector<Obstacle*> obstacles;
   
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

const std::vector<Obstacle*> Map::getObstacles() const
{
   return obstacles;
}

bool** Map::getPassibilityMatrix() const
{
   return passibilityMap;
}

void Map::step(long timePassed) const
{
   std::vector<Obstacle*>::const_iterator iter;
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
}
