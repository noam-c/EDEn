/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Tileset.h"

#include <algorithm>
#include <fstream>

#include "tinyxml.h"

#include "GraphicsUtil.h"

#include "Texture.h"
#include "TileEngine.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_RES_LOAD | DEBUG_TILE_ENG

Tileset::Tileset(ResourceKey name) :
   Resource(name),
   m_texture(nullptr)
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
   if(imageElement == nullptr)
   {
      DEBUG("Expected image data in tileset.");
      T_T("Failed to parse tileset data.");
   }

   std::string imagePath = imageElement->Attribute("source");
   DEBUG("Loading tileset image \"%s\"...", imagePath.c_str());

   m_texture.reset(new Texture(std::string("data/tilesets/") + imagePath));
   m_size = m_texture->getSize() / TileEngine::TILE_SIZE;

   m_collisionShapes.resize(m_size.getArea());

   auto const* tileElement = root->FirstChildElement("tile");
   for(;tileElement != nullptr; tileElement = tileElement->NextSiblingElement("tile"))
   {
      int tileNum = -1;
      tileElement->Attribute("id", &tileNum);

      if(tileNum < 0)
      {
         continue;
      }

      auto& collisionShape = m_collisionShapes[tileNum];

      // Retrieve the collision shape information for this tile
      const auto objectGroupElement = tileElement->FirstChildElement("objectgroup");

      if(objectGroupElement)
      {
         const auto collisionElement = objectGroupElement->FirstChildElement("object");
         if(collisionElement)
         {
            collisionShape.left = std::stoi(collisionElement->Attribute("x")) / TileEngine::TILE_SIZE;
            collisionShape.top = std::stoi(collisionElement->Attribute("y")) / TileEngine::TILE_SIZE;

            const auto collisionWidth = std::stoi(collisionElement->Attribute("width")) / TileEngine::TILE_SIZE;
            const auto collisionHeight = std::stoi(collisionElement->Attribute("height")) / TileEngine::TILE_SIZE;

            collisionShape.right = collisionShape.left + collisionWidth;
            collisionShape.bottom = collisionShape.top + collisionHeight;
         }
      }
   }
}

void Tileset::draw(int destX, int destY, int tileNum, bool useAlphaTesting)
{
   int tilesetX = tileNum % m_size.width;
   int tilesetY = tileNum / m_size.width;

   float destLeft = float(destX * TileEngine::TILE_SIZE);
   float destRight = float((destX + 1) * TileEngine::TILE_SIZE);
   float destTop = float(destY * TileEngine::TILE_SIZE);
   float destBottom = float((destY + 1) * TileEngine::TILE_SIZE);

   float tileRight = float((tilesetX + 1) * TileEngine::TILE_SIZE - 1);
   float tileBottom = float((tilesetY + 1) * TileEngine::TILE_SIZE - 1);

   float top = float(tilesetY) / m_size.height;
   float bottom = float(tileBottom) / (m_size.height * TileEngine::TILE_SIZE - 1);
   float left = float(tilesetX) / m_size.width;
   float right = float(tileRight) / (m_size.width * TileEngine::TILE_SIZE - 1);

   glPushAttrib(GL_COLOR_BUFFER_BIT);

   if(useAlphaTesting)
   {
      // NOTE: Alpha testing doesn't do transparency; it either draws a pixel or it doesn't
      // If we want partial transparency, we would need to use alpha blending

       // Enable alpha testing
      glEnable(GL_ALPHA_TEST);

      // Set the alpha blending evaluation function
      glAlphaFunc(GL_GREATER, 0.1f);
   }

   m_texture->bind();

   glBegin(GL_QUADS);
      glTexCoord2f(left, top); glVertex3f(destLeft, destTop, 0.0f);
      glTexCoord2f(right, top); glVertex3f(destRight, destTop, 0.0f);
      glTexCoord2f(right, bottom); glVertex3f(destRight, destBottom, 0.0f);
      glTexCoord2f(left, bottom); glVertex3f(destLeft, destBottom, 0.0f);
   glEnd();

   glPopAttrib();
}

void Tileset::drawColorToTile(int destX, int destY, float r, float g, float b, float a)
{
   float destLeft = float(destX * TileEngine::TILE_SIZE);
   float destRight = float((destX + 1) * TileEngine::TILE_SIZE);
   float destTop = float(destY * TileEngine::TILE_SIZE);
   float destBottom = float((destY + 1) * TileEngine::TILE_SIZE);

   glPushAttrib(GL_ENABLE_BIT);
   glEnable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);

   // Save old glBlendFunc values.
   // There is no bit in glPushAttrib that would preserve them, so
   // do it the old(er)-school way.
   int oldBlendSrc;
   int oldBlendDst;
   glGetIntegerv(GL_BLEND_SRC, &oldBlendSrc);
   glGetIntegerv(GL_BLEND_DST, &oldBlendDst);

   glBlendFunc(GL_ONE, GL_DST_ALPHA);

   glBegin(GL_QUADS);
      glColor4f(r, g, b, a);
      glVertex3f(destLeft, destTop, 0.0f);
      glVertex3f(destRight, destTop, 0.0f);
      glVertex3f(destRight, destBottom, 0.0f);
      glVertex3f(destLeft, destBottom, 0.0f);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glEnd();
   
   // Restore all OpenGL state
   glBlendFunc(oldBlendSrc, oldBlendDst);
   glPopAttrib();
}

geometry::Rectangle Tileset::getCollisionRect(int tileNum) const
{
   return m_collisionShapes[tileNum];
}