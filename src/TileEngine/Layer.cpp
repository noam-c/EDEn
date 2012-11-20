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

Layer::Layer(const TiXmlElement* layerData, const shapes::Rectangle& bounds) :
   bounds(bounds),
   heightOffset(0)
{
   const TiXmlElement* propertiesElement = layerData->FirstChildElement("properties");

   const TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
   while(propertyElement != NULL)
   {
      if(std::string(propertyElement->Attribute("name")) == "tilesetName")
      {
         std::string tilesetName = propertyElement->Attribute("value");
         tileset = ResourceLoader::getTileset(tilesetName);
      }
      else if(std::string(propertyElement->Attribute("name")) == "heightOffset")
      {
         propertyElement->Attribute("value", &heightOffset);
         DEBUG("Height offset found: %d", heightOffset);
         if(heightOffset < 0) heightOffset = 0;
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
      if (y < heightOffset || y >= height - heightOffset)
      {
         for(unsigned int x = 0; x < width; ++x)
         {
            tileMap[y][x] = -1;
         }
      }
   }

   for(unsigned int y = 0; y < height - heightOffset; ++y)
   {
      for(unsigned int x = 0; x < width; ++x)
      {
         std::string entry;
         std::getline(layerStream, entry, ',');
         tileMap[y + heightOffset][x] = atoi(entry.c_str()) - 1;
      }
   }
}

Layer::~Layer()
{
   const unsigned int height = bounds.getHeight();
   for(unsigned int i = 0; i < height; ++i)
   {
      delete [] tileMap[i];
   }

   delete [] tileMap;
}

void Layer::draw(int row, bool isForeground) const
{
   const unsigned int width = bounds.getWidth();
   for(unsigned int column = 0; column < width; ++column)
   {
      const int tileNum = tileMap[row][column];
      if(tileNum != -1)
      {
         tileset->draw(column, row - heightOffset, tileNum, isForeground);
      }
   }
}
