/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Layer.h"
#include "Rectangle.h"
#include "ResourceLoader.h"
#include "Tileset.h"
#include "tinyxml.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TILE_ENG;

Layer::Layer(const TiXmlElement* layerData, const shapes::Rectangle& bounds) :
   m_bounds(bounds),
   m_heightOffset(0)
{
   const TiXmlElement* propertiesElement = layerData->FirstChildElement("properties");

   const TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
   while(propertyElement != nullptr)
   {
      if(std::string(propertyElement->Attribute("name")) == "tilesetName")
      {
         std::string tilesetName = propertyElement->Attribute("value");
         m_tileset = ResourceLoader::getTileset(tilesetName);
      }
      else if(std::string(propertyElement->Attribute("name")) == "heightOffset")
      {
         propertyElement->Attribute("value", &m_heightOffset);
         DEBUG("Height offset found: %d", m_heightOffset);
         if(m_heightOffset < 0) m_heightOffset = 0;
      }

      propertyElement = propertyElement->NextSiblingElement("property");
   }

   if(m_tileset == nullptr)
   {
      T_T("Layer doesn't contain a tileset.");
   }

   DEBUG("Loading layer data.");
   const TiXmlText* layerDataElement = layerData->FirstChildElement("data")->FirstChild()->ToText();
   std::stringstream layerStream(layerDataElement->Value());

   const unsigned int width = bounds.getWidth();
   const unsigned int height = bounds.getHeight();

   m_tileMap = new int*[height];
   for(unsigned int y = 0; y < height; ++y)
   {
      m_tileMap[y] = new int[width];
      if (y < m_heightOffset || y >= height - m_heightOffset)
      {
         for(unsigned int x = 0; x < width; ++x)
         {
            m_tileMap[y][x] = -1;
         }
      }
   }

   for(unsigned int y = 0; y < height - m_heightOffset; ++y)
   {
      for(unsigned int x = 0; x < width; ++x)
      {
         std::string entry;
         std::getline(layerStream, entry, ',');
         m_tileMap[y + m_heightOffset][x] = atoi(entry.c_str()) - 1;
      }
   }
}

Layer::~Layer()
{
   const unsigned int height = m_bounds.getHeight();
   for(unsigned int i = 0; i < height; ++i)
   {
      delete [] m_tileMap[i];
   }

   delete [] m_tileMap;
}

void Layer::draw(int row, bool isForeground) const
{
   const unsigned int width = m_bounds.getWidth();
   for(unsigned int column = 0; column < width; ++column)
   {
      const int tileNum = m_tileMap[row][column];
      if(tileNum != -1)
      {
         m_tileset->draw(column, row - m_heightOffset, tileNum, isForeground);
      }
   }
}
