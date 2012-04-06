/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Tileset.h"
#include "SDL_opengl.h"
#include <fstream>
#include "tinyxml.h"
#include "GraphicsUtil.h"
#include "TileEngine.h"
#include <algorithm>
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_TILE_ENG;

Tileset::Tileset(ResourceKey name) : Resource(name)
{
}

void Tileset::load(const std::string& path)
{
   DEBUG("Loading tileset file %s", path.c_str());
   
   std::ifstream input(path.c_str());
   if(!input)
   {
      T_T("Failed to open tileset file for reading.");
   }
   
   TiXmlDocument xmlDoc;
   input >> xmlDoc;
   
   if(xmlDoc.Error())
   {
      DEBUG("Error occurred in tileset XML parsing: %s", xmlDoc.ErrorDesc());
      T_T("Failed to parse tileset data.");
   }
   
   TiXmlElement* root = xmlDoc.RootElement();
   if(strcmp(root->Value(), "tileset") != 0)
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse tileset data.");
   }
   
   TiXmlElement* imageElement = root->FirstChildElement("image");
   if(imageElement == NULL)
   {
      DEBUG("Expected image data in tileset.");
      T_T("Failed to parse tileset data.");
   }
   
   std::string imagePath = imageElement->Attribute("source");
   DEBUG("Loading tileset image \"%s\"...", imagePath.c_str());

   int imgWidth, imgHeight;
   GraphicsUtil::getInstance()->loadGLTexture((std::string("data/tilesets/") + imagePath).c_str(), texture, imgWidth, imgHeight);

   width = imgWidth / TileEngine::TILE_SIZE;
   height = imgHeight / TileEngine::TILE_SIZE;

   // \todo When Tiled makes it easier, the tileset data needs to hold the default
   // passibility matrix for a Tileset
   // Since maps will rarely change the passibility of tiles, it doesn't make
   // sense to hold passibility within each map's data; a map's Lua script
   // should be able to cheat default passibility if necessary.
   passibility.resize(width * height);
   std::fill(passibility.begin(), passibility.end(), true);
   
   TiXmlElement* tileElement = root->FirstChildElement("tile");
   while(tileElement != NULL)
   {
      int tileNum = -1;
      tileElement->Attribute("id", &tileNum);

      TiXmlElement* propertiesElement = tileElement->FirstChildElement("properties");

      TiXmlElement* propertyElement = propertiesElement->FirstChildElement("property");
      while(propertyElement != NULL)
      {
         if(std::string(propertyElement->Attribute("name")) == "collision")
         {
            const std::string collision = propertyElement->Attribute("value");
            passibility[tileNum] = collision != "true";
            break;
         }
         
         propertyElement = propertyElement->NextSiblingElement("property");
      }
      
      tileElement = tileElement->NextSiblingElement("tile");
   }
}
   
void Tileset::draw(int destX, int destY, int tileNum)
{
   int tilesetX = tileNum % width;
   int tilesetY = tileNum / width;

   float destLeft = float(destX * TileEngine::TILE_SIZE);
   float destRight = float((destX + 1) * TileEngine::TILE_SIZE);
   float destTop = float(destY * TileEngine::TILE_SIZE);
   float destBottom = float((destY + 1) * TileEngine::TILE_SIZE);

   float tileRight = float((tilesetX + 1) * TileEngine::TILE_SIZE - 1);
   float tileBottom = float((tilesetY + 1) * TileEngine::TILE_SIZE - 1);

   float top = float(tilesetY) / height;
   float bottom = float(tileBottom) / (height * TileEngine::TILE_SIZE - 1);
   float left = float(tilesetX) / width;
   float right = float(tileRight) / (width * TileEngine::TILE_SIZE - 1);

   glBindTexture(GL_TEXTURE_2D, texture);

   glBegin(GL_QUADS);
      glTexCoord2f(left, top); glVertex3f(destLeft, destTop, 0.0f);
      glTexCoord2f(right, top); glVertex3f(destRight, destTop, 0.0f);
      glTexCoord2f(right, bottom); glVertex3f(destRight, destBottom, 0.0f);
      glTexCoord2f(left, bottom); glVertex3f(destLeft, destBottom, 0.0f);
   glEnd();
}

void Tileset::drawColorToTile(int destX, int destY, float r, float g, float b)
{
   float destLeft = float(destX * TileEngine::TILE_SIZE);
   float destRight = float((destX + 1) * TileEngine::TILE_SIZE);
   float destTop = float(destY * TileEngine::TILE_SIZE);
   float destBottom = float((destY + 1) * TileEngine::TILE_SIZE);

   glDisable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
      glColor3f(r, g, b);
      glVertex3f(destLeft, destTop, 0.0f);
      glVertex3f(destRight, destTop, 0.0f);
      glVertex3f(destRight, destBottom, 0.0f);
      glVertex3f(destLeft, destBottom, 0.0f);
      glColor3f(1.0f, 1.0f, 1.0f);
   glEnd();
   glEnable(GL_TEXTURE_2D);
}

size_t Tileset::getSize()
{
   return sizeof(this);
}

bool Tileset::isPassible(int tileNum) const
{
   return passibility[tileNum];
}

Tileset::~Tileset()
{
   glDeleteTextures(1, &texture);
}
