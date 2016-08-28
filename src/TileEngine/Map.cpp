/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Map.h"

#include "EnumUtils.h"
#include "Layer.h"
#include "Pathfinder.h"
#include "ResourceLoader.h"
#include "TileEngine.h"
#include "Tileset.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_RES_LOAD | DEBUG_ENTITY_GRID

// Define as 1 to have the map rendering highlight the map's impassible terrain
#define DRAW_IMPASSIBILITY 0

Map::Map(const std::string& name, const std::string& filePath) :
   m_name(name)
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

   m_bounds = geometry::Rectangle(geometry::Point2D::ORIGIN, geometry::Size(width, height));

   const TiXmlElement* layerElement = root->FirstChildElement("layer");
   while(layerElement != nullptr)
   {
      const std::string layerName(layerElement->Attribute("name"));
      if(layerName == "background")
      {
         // This 'new' is being emplaced into a unique_ptr and therefore
         // cleanup will be handled automatically.
         m_backgroundLayers.emplace_back(new Layer(layerElement, m_bounds));
         DEBUG("Background layer added.");
      }
      else if(layerName == "foreground")
      {
         // This 'new' is being emplaced into a unique_ptr and therefore
         // cleanup will be handled automatically.
         m_foregroundLayers.emplace_back(new Layer(layerElement, m_bounds));
         DEBUG("Foreground layer added.");
      }

      layerElement = layerElement->NextSiblingElement("layer");
   }

   initializePassibilityMatrix();
   
   bool hasCollisionLayer = false;
   bool hasEntrancesLayer = false;
   bool hasExitsLayer = false;
   bool hasTriggersLayer = false;
   bool hasNPCLayer = false;

   auto const* objectGroupElement = root->FirstChildElement("objectgroup");
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
      else if(objectGroupName == "npcs" && !hasNPCLayer)
      {
         parseNPCGroup(objectGroupElement);
         hasNPCLayer = true;
      }

      objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
   }

   DEBUG("Map loaded.");
}

Map::~Map() = default;

void Map::addCollisionRect(const geometry::Rectangle& rect)
{
   m_passibilityMap.fillRect(rect, 0);
}

void Map::parseCollisionGroup(const TiXmlElement* collisionGroupElement)
{
   DEBUG("Loading collision layer.");
   if(collisionGroupElement != nullptr)
   {
      geometry::Point2D topLeft;
      int width;
      int height;

      const TiXmlElement* objectElement = collisionGroupElement->FirstChildElement("object");
      while(objectElement != nullptr)
      {
         objectElement->Attribute("x", &topLeft.x);
         objectElement->Attribute("y", &topLeft.y);
         objectElement->Attribute("width", &width);
         objectElement->Attribute("height", &height);

         geometry::Rectangle rect(topLeft / TileEngine::TILE_SIZE, geometry::Size(width, height) / TileEngine::TILE_SIZE);
         DEBUG("Found collision object at %d,%d with width %d and height %d.", rect.left, rect.top, rect.getWidth(), rect.getHeight());
         addCollisionRect(rect);

         objectElement = objectElement->NextSiblingElement("object");
      }
   }
}

void Map::parseMapEntrancesGroup(const TiXmlElement* entrancesGroupElement)
{
   DEBUG("Loading entrance layer.");
   if(entrancesGroupElement != nullptr)
   {
      geometry::Point2D entrance;

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
      geometry::Point2D topLeft;
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

         geometry::Rectangle rect(topLeft, geometry::Size(width, height));
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
   DEBUG("Loading map trigger layer.");
   if(triggersGroupElement != nullptr)
   {
      geometry::Point2D topLeft;
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

         geometry::Rectangle rect(topLeft, geometry::Size(width, height));
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

void Map::parseNPCGroup(const TiXmlElement* npcGroupElement)
{
   DEBUG("Loading NPC layer.");
   if(npcGroupElement != nullptr)
   {
      geometry::Point2D topLeft;
      geometry::Size size;
      int width;
      int height;
      std::string npcName;
      std::string spritesheet;
      geometry::Direction direction;
      
      const TiXmlElement* objectElement = npcGroupElement->FirstChildElement("object");
      while(objectElement != nullptr)
      {
         objectElement->Attribute("x", &topLeft.x);
         objectElement->Attribute("y", &topLeft.y);

         // Attribute doesn't support unsigned ints, so we need to cast explicitly.
         objectElement->Attribute("width", &width);
         objectElement->Attribute("height", &height);
         size =
         {
            static_cast<unsigned int>(std::max(width, 0)),
            static_cast<unsigned int>(std::max(height, 0))
         };

         npcName = objectElement->Attribute("name");
         direction = geometry::Direction::DOWN;
         
         const auto propertiesElement = objectElement->FirstChildElement("properties");
         auto propertyElement = propertiesElement->FirstChildElement("property");
         while(propertyElement != nullptr)
         {
            if(std::string(propertyElement->Attribute("name")) == "spritesheet")
            {
               spritesheet = propertyElement->Attribute("value");
               break;
            }
            else if(std::string(propertyElement->Attribute("name")) == "direction")
            {
               direction = geometry::toDirection(propertyElement->Attribute("value"));
               if(direction == geometry::Direction::NONE)
               {
                  direction = geometry::Direction::DOWN;
               }
            }
            
            propertyElement = propertyElement->NextSiblingElement("property");
         }
         
         DEBUG("Found NPC %s at %d,%d with spritesheet %s, width %d and height %d.",
               npcName.c_str(), topLeft.x, topLeft.y, spritesheet.c_str(), size.width, size.height);

         if(size.width > 0 &&
            size.height > 0 &&
            !npcName.empty() &&
            !spritesheet.empty())
         {
            m_npcsToSpawn.emplace_back(NPCSpawnPoint{npcName, spritesheet, topLeft, size, direction});
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
   
   m_passibilityMap.resize(size, 1);

   const auto processRect = [this](const geometry::Rectangle& rect) {
      addCollisionRect(rect);
   };

   for(const auto& layers : {&m_backgroundLayers, &m_foregroundLayers}) {
      for(const auto& layer : *layers) {
         layer->forEachCollisionRect(processRect);
      }
   }
}

const std::string& Map::getName() const
{
   return m_name;
}

const geometry::Rectangle& Map::getBounds() const
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

const std::vector<Map::NPCSpawnPoint>& Map::getNPCSpawnPoints() const
{
   return m_npcsToSpawn;
}

const geometry::Point2D& Map::getMapEntrance(const std::string& previousMap) const
{
   const auto& result = m_mapEntrances.find(previousMap);

   if(result == m_mapEntrances.end())
   {
      return geometry::Point2D::ORIGIN;
   }

   return result->second;
}

void Map::step(long timePassed) const
{
}

void Map::drawBackground(int row) const
{
   bool firstLayer = true;
   for(const auto& layer : m_backgroundLayers)
   {
      layer->draw(row, !firstLayer);
      firstLayer = false;
   }
}

void Map::drawForeground(int row) const
{
   for(const auto& layer : m_foregroundLayers)
   {
      layer->draw(row, true);
   }

   if(DRAW_IMPASSIBILITY)
   {
      const unsigned int width = m_bounds.getWidth();
      
      for(unsigned int column = 0; column < width; ++column)
      {
         if(!isPassible(column, row))
         {
            Tileset::drawColorToTile(column, row, 1.0f, 0.0f, 0.0f, 0.2f);
         }
      }
   }
}
