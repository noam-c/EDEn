/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Layer.h"
#include "Rectangle.h"
#include "ResourceLoader.h"
#include "Tileset.h"
#include "tinyxml.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TILE_ENG;

Layer::Layer(const TiXmlElement* layerData, const shapes::Rectangle& bounds) : bounds(bounds)
{
   const TiXmlElement* propertiesElement = layerData->FirstChildElement("properties");

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
      T_T("Layer doesn't contain a tileset.");
   }

   DEBUG("Loading layer data.");
   const TiXmlText* layerDataElement = layerData->FirstChildElement("data")->FirstChild()->ToText();
   std::stringstream layerStream(layerDataElement->Value());

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

void Layer::draw(bool isForeground) const
{
   const unsigned int width = bounds.getWidth();
   const unsigned int height = bounds.getHeight();
   for(unsigned int i = 0; i < width; ++i)
   {
      for(unsigned int j = 0; j < height; ++j)
      {
         const int tileNum = tileMap[j][i];
         if(tileNum != -1)
         {
            tileset->draw(i, j, tileNum, isForeground);
         }
      }
   }
}

Layer::~Layer()
{
   const unsigned int width = bounds.getWidth();
   for(unsigned int i = 0; i < width; ++i)
   {
      delete [] tileMap[i];
   }

   delete [] tileMap;
}

