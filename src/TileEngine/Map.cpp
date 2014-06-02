/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Map.h"
#include "Tileset.h"
#include "Pathfinder.h"
#include "ResourceLoader.h"
#include "TileEngine.h"
#include "Size.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_ENTITY_GRID;

// Define as 1 to have the game draw the map's passibility matrix
#define DRAW_PASSIBILITY 0

Map::Map(const std::string& name, const std::string& filePath) : m_name(name)
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
   
   m_bounds = shapes::Rectangle(shapes::Point2D::ORIGIN, shapes::Size(width, height));
   
   const TiXmlElement* layerElement = root->FirstChildElement("layer");
   while(layerElement != nullptr)
   {
      const std::string layerName(layerElement->Attribute("name"));
      if(layerName == "background")
      {
         m_backgroundLayers.emplace_back(new Layer(layerElement, m_bounds));
         DEBUG("Background layer added.");
      }
      else if(layerName == "foreground")
      {
         m_foregroundLayers.emplace_back(new Layer(layerElement, m_bounds));
         DEBUG("Foreground layer added.");
      }
      
      layerElement = layerElement->NextSiblingElement("layer");
   }

   bool hasCollisionLayer = false;
   bool hasEntrancesLayer = false;
   bool hasExitsLayer = false;
   bool hasTriggersLayer = false;

   const TiXmlElement* objectGroupElement = root->FirstChildElement("objectgroup");
   while(objectGroupElement != nullptr)
   {
      std::string objectGroupName = objectGroupElement->Attribute("name");
      if(objectGroupName == "collision" && !hasCollisionLayer)
      {
         parseCollisionGroup(objectGroupElement);
         hasCollisionLayer = true;
      }
      else if(objectGroupName == "entrances" && !hasEntrancesLayer)
      {
         parseMapEntrancesGroup(objectGroupElement);
         hasEntrancesLayer = true;
      }
      else if(objectGroupName == "exits" && !hasExitsLayer)
      {
         parseMapExitsGroup(objectGroupElement);
         hasExitsLayer = true;
      }
      else if(objectGroupName == "triggers" && !hasTriggersLayer)
      {
         parseMapTriggersGroup(objectGroupElement);
         hasTriggersLayer = true;
      }
      
      
      objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
   }
   
   DEBUG("Map loaded.");
}

void Map::parseCollisionGroup(const TiXmlElement* collisionGroupElement)
{
   initializePassibilityMatrix();

   DEBUG("Loading collision layer.");
   if(collisionGroupElement != nullptr)
   {
      shapes::Point2D topLeft;
      int width;
      int height;

      const TiXmlElement* objectElement = collisionGroupElement->FirstChildElement("object");
      while(objectElement != nullptr)
      {
         objectElement->Attribute("x", &topLeft.x);
         objectElement->Attribute("y", &topLeft.y);
         objectElement->Attribute("width", &width);
         objectElement->Attribute("height", &height);
         
         shapes::Rectangle rect(topLeft / TileEngine::TILE_SIZE, shapes::Size(width, height) / TileEngine::TILE_SIZE);
         DEBUG("Found collision object at %d,%d with width %d and height %d.", rect.left, rect.top, rect.getWidth(), rect.getHeight());
         if(rect.getWidth() > 0 && rect.getHeight() > 0)
         {
            for(int y = rect.top; y < rect.bottom; ++y)
            {
               for(int x = rect.left; x < rect.right; ++x)
               {
                  m_passibilityMap(x, y) = 0;
               }  
            }
         }

         objectElement = objectElement->NextSiblingElement("object");
      }
   }
}

void Map::parseMapEntrancesGroup(const TiXmlElement* entrancesGroupElement)
{
   DEBUG("Loading entrance layer.");
   if(entrancesGroupElement != nullptr)
   {
      shapes::Point2D entrance;

      const TiXmlElement* objectElement = entrancesGroupElement->FirstChildElement("object");
      while(objectElement != nullptr)
      {
         objectElement->Attribute("x", &entrance.x);
         objectElement->Attribute("y", &entrance.y);
         std::string previousMap;

         const TiXmlElement* propertiesElement = objectElement->FirstChildElement("properties");
         const TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
         while(propertyElement != nullptr)
         {
            if(std::string(propertyElement->Attribute("name")) == "entrance")
            {
               previousMap = propertyElement->Attribute("value");
               break;
            }

            propertyElement = propertyElement->NextSiblingElement("property");
         }

         DEBUG("Found entrance from %s at %d,%d.",
               previousMap.c_str(), entrance.x, entrance.y);

         if(!previousMap.empty())
         {
            m_mapEntrances[previousMap] = entrance;
         }

         objectElement = objectElement->NextSiblingElement("object");
      }
   }
}

void Map::parseMapExitsGroup(const TiXmlElement* exitsGroupElement)
{
   DEBUG("Loading exit layer.");
   if(exitsGroupElement != nullptr)
   {
      shapes::Point2D topLeft;
      int width;
      int height;
      
      const TiXmlElement* objectElement = exitsGroupElement->FirstChildElement("object");
      while(objectElement != nullptr)
      {
         objectElement->Attribute("x", &topLeft.x);
         objectElement->Attribute("y", &topLeft.y);
         objectElement->Attribute("width", &width);
         objectElement->Attribute("height", &height);
         std::string nextMap;
         
         const TiXmlElement* propertiesElement = objectElement->FirstChildElement("properties");
         const TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
         while(propertyElement != nullptr)
         {
            if(std::string(propertyElement->Attribute("name")) == "exit")
            {
               nextMap = propertyElement->Attribute("value");
               break;
            }
            
            propertyElement = propertyElement->NextSiblingElement("property");
         }
         
         shapes::Rectangle rect(topLeft, shapes::Size(width, height));
         DEBUG("Found exit %s at %d,%d with width %d and height %d.",
               nextMap.c_str(), topLeft.x, topLeft.y, rect.getWidth(), rect.getHeight());
         
         if(rect.getWidth() > 0 && rect.getHeight() > 0 && !nextMap.empty())
         {
            m_mapExits.emplace_back(nextMap, rect);
         }
         
         objectElement = objectElement->NextSiblingElement("object");
      }
   }
}

void Map::parseMapTriggersGroup(const TiXmlElement* triggersGroupElement)
{
   DEBUG("Loading exit layer.");
   if(triggersGroupElement != nullptr)
   {
      shapes::Point2D topLeft;
      int width;
      int height;
      
      const TiXmlElement* objectElement = triggersGroupElement->FirstChildElement("object");
      while(objectElement != nullptr)
      {
         objectElement->Attribute("x", &topLeft.x);
         objectElement->Attribute("y", &topLeft.y);
         objectElement->Attribute("width", &width);
         objectElement->Attribute("height", &height);
         std::string name;
         
         const TiXmlElement* propertiesElement = objectElement->FirstChildElement("properties");
         const TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
         while(propertyElement != nullptr)
         {
            if(std::string(propertyElement->Attribute("name")) == "trigger")
            {
               name = propertyElement->Attribute("value");
               break;
            }
            
            propertyElement = propertyElement->NextSiblingElement("property");
         }
         
         shapes::Rectangle rect(topLeft, shapes::Size(width, height));
         DEBUG("Found trigger %s at %d,%d with width %d and height %d.",
               name.c_str(), topLeft.x, topLeft.y, rect.getWidth(), rect.getHeight());
         
         if(rect.getWidth() > 0 && rect.getHeight() > 0)
         {
            m_triggerZones.emplace_back(name, rect);
         }
         
         objectElement = objectElement->NextSiblingElement("object");
      }
   }
}

bool Map::isPassible(int x, int y) const
{
    return m_passibilityMap(x, y) == 1;
}

void Map::initializePassibilityMatrix()
{
   const auto& size = m_bounds.getSize();
   const auto& width = size.width;
   const auto& height = size.height;

   m_passibilityMap.resize(size);
   for(unsigned int x = 0; x < width; ++x)
   {
      for(unsigned int y = 0; y < height; ++y)
      {
         m_passibilityMap(x, y) = 1;
      }
   }
}

const std::string& Map::getName() const
{
   return m_name;
}

const shapes::Rectangle& Map::getBounds() const
{
   return m_bounds;
}

const std::vector<TriggerZone>& Map::getTriggerZones() const
{
   return m_triggerZones;
}

const std::vector<MapExit>& Map::getMapExits() const
{
   return m_mapExits;
}

const shapes::Point2D& Map::getMapEntrance(const std::string& previousMap) const
{
   const auto& result = m_mapEntrances.find(previousMap);

   if(result == m_mapEntrances.end())
   {
      return shapes::Point2D::ORIGIN;
   }

   return result->second;
}

void Map::step(long timePassed) const
{
}

void Map::drawBackground(int row) const
{
   if(DRAW_PASSIBILITY)
   {
      const unsigned int width = m_bounds.getWidth();

      for(unsigned int column = 0; column < width; ++column)
      {
         if(isPassible(column, row))
         {
            Tileset::drawColorToTile(column, row, 0.0f, 1.0f, 0.0f);
         }
         else
         {
            Tileset::drawColorToTile(column, row, 1.0f, 0.0f, 0.0f);
         }
      }
   }
   else
   {
      bool firstLayer = true;
      for(const auto& layer : m_backgroundLayers)
      {
         layer->draw(row, !firstLayer);
         firstLayer = false;
      }
   }
}

void Map::drawForeground(int row) const
{
   if(!DRAW_PASSIBILITY)
   {
      for(const auto& layer : m_foregroundLayers)
      {
         layer->draw(row, true);
      }
   }
}
