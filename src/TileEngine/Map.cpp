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
#include "Rectangle.h"
#include "Point2D.h"
#include <sstream>

#include "DebugUtils.h"

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
   
   const TiXmlElement* root = xmlDoc.RootElement();
   if(strcmp(root->Value(), "map") != 0)
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse map data.");
   }
   
   int width;
   int height;
   root->Attribute("width", &width);
   root->Attribute("height", &height);
   
   bounds = shapes::Rectangle(shapes::Point2D::ORIGIN, shapes::Size(width, height));
   
   const TiXmlElement* propertiesElement = root->FirstChildElement("properties");
   
   const TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
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
   
   const TiXmlElement* floorElement = root->FirstChildElement("layer");
   while(floorElement != NULL)
   {
      if(std::string(floorElement->Attribute("name")) == "floor")
      {
         break;
      }
      
      floorElement = floorElement->NextSiblingElement("layer");
   }
   
   if(floorElement == NULL)
   {
      DEBUG("Expected layer data in map.");
      T_T("Failed to parse map data.");
   }

   parseFloorLayer(floorElement);

   const TiXmlElement* collisionElement = root->FirstChildElement("objectgroup");
   while(floorElement != NULL)
   {
      if(std::string(collisionElement->Attribute("name")) == "collision")
      {
         break;
      }
      
      collisionElement = collisionElement->NextSiblingElement("objectgroup");
   }

   parseCollisionGroup(collisionElement);
   
   std::vector<Obstacle*> obstacles;
   
   DEBUG("Map loaded.");
}

void Map::parseFloorLayer(const TiXmlElement* floorLayerElement)
{
   const TiXmlText* floorDataElement = floorLayerElement->FirstChildElement("data")->FirstChild()->ToText();
   std::stringstream layerStream(floorDataElement->Value());
   
   const unsigned int width = bounds.getWidth();
   const unsigned int height = bounds.getHeight();

   tileMap = new int*[height];
   for(unsigned int y = 0; y < height; ++y)
   {
      tileMap[y] = new int[width];
      
      for(unsigned int x = 0; x < width; ++x)
      {
         std::string entry;
         std::getline(layerStream, entry, ',');
         tileMap[y][x] = atoi(entry.c_str()) - 1;
      }
   }
}

void Map::parseCollisionGroup(const TiXmlElement* collisionGroupElement)
{
   initializePassibilityMatrix();

   if(collisionGroupElement != NULL)
   {
      shapes::Point2D topLeft;
      int width;
      int height;

      const TiXmlElement* objectElement = collisionGroupElement->FirstChildElement("object");
      while(objectElement != NULL)
      {
         objectElement->Attribute("x", &topLeft.x);
         objectElement->Attribute("y", &topLeft.y);
         objectElement->Attribute("width", &width);
         objectElement->Attribute("height", &height);
         
         shapes::Rectangle rect(topLeft / TileEngine::TILE_SIZE, shapes::Size(width, height) / TileEngine::TILE_SIZE);
         if(rect.getWidth() > 0 && rect.getHeight() > 0)
         {
            for(int y = rect.top; y < rect.bottom; ++y)
            {
               for(int x = rect.left; x < rect.right; ++x)
               {
                  passibilityMap[y][x] = false;
               }  
            }
         }

         objectElement = objectElement->NextSiblingElement("object");
      }
   }
}

bool Map::isPassible(int x, int y) const
{
    return passibilityMap[y][x];
}

void Map::initializePassibilityMatrix()
{
   const unsigned int width = bounds.getWidth();
   const unsigned int height = bounds.getHeight();

   passibilityMap = new bool*[height];
   for(unsigned int y = 0; y < height; ++y)
   {
      passibilityMap[y] = new bool[width];
      for(unsigned int x = 0; x < width; ++x)
      {
         passibilityMap[y][x] = tileset->isPassible(tileMap[y][x]);
      }
   }
}

std::string Map::getName() const
{
   return mapName;
}

const shapes::Rectangle& Map::getBounds() const
{
   return bounds;
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
   const unsigned int width = bounds.getWidth();
   const unsigned int height = bounds.getHeight();
   
   for(unsigned int i = 0; i < width; ++i)
   {
      for(unsigned int j = 0; j < height; ++j)
      {
#ifdef DRAW_PASSIBILITY
         if(isPassible(i,j))
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
   const unsigned int width = bounds.getWidth();
   for(unsigned int i = 0; i < width; ++i)
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
