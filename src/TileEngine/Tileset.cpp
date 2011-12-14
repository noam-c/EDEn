/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Tileset.h"
#include "GLInclude.h"
#include <fstream>
#include "GraphicsUtil.h"
#include "TileEngine.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_RES_LOAD | DEBUG_TILE_ENG;

const std::string Tileset::IMG_EXTENSION = ".png";
const std::string Tileset::DATA_EXTENSION = ".edt";

Tileset::Tileset(ResourceKey name) : Resource(name)
{
}

void Tileset::load(const char* path)
{
   std::string imagePath(path);
   imagePath += IMG_EXTENSION;

   DEBUG("Loading tileset image \"%s\"...", imagePath.c_str());

   int imgWidth, imgHeight;
   GraphicsUtil::getInstance()->loadGLTexture(imagePath.c_str(), texture, imgWidth, imgHeight);

   width = imgWidth / TileEngine::TILE_SIZE;
   height = imgHeight / TileEngine::TILE_SIZE;

   // A data file is needed to hold the default passibility matrix for a Tileset
   // Since maps will rarely change the passibility of tiles, it doesn't make
   // sense to hold passibility within each map's data; a map's Lua script
   // should be able to cheat default passibility if necessary.
   std::string dataPath(path);
   dataPath += DATA_EXTENSION;
   DEBUG("Loading tileset data \"%s\"...", dataPath.c_str());

   std::ifstream in(dataPath.c_str());

   if(!in.is_open())
   {  
      T_T(std::string("Error opening file: ") + path);
   }

   passibility = new bool*[width];
   for(int i = 0; i < width; ++i)
   {
      passibility[i] = new bool[height];
      for(int j = 0; j < height; ++j)
      {
         if(in)
         {
            int c;
            in >> c;
            passibility[i][j] = (c != 0);
         }
         else
         {
            T_T("Tileset has incomplete passibility matrix.");
         }
      }
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
   int tilesetX = tileNum % width;
   int tilesetY = tileNum / width;

   return passibility[tilesetX][tilesetY];
}

Tileset::~Tileset()
{
   for(int i = 0; i < width; ++i)
   {
      delete [] passibility[i];
   }

   delete [] passibility;

   glDeleteTextures(1, &texture);
}
