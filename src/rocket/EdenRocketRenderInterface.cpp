/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

/*
 * This source file is based on the EdenRocketRenderInterface class.
 * It has been heavily modified to work with the EDEn codebase.
 *
 * EdenRocketRenderInterface.h and EdenRocketRenderInterface.cpp are
 * part of libRocket, the HTML/CSS Interface Middleware.
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 */

#include <EdenRocketRenderInterface.h>
#include <RmlUi/Core.h>

#include "GraphicsUtil.h"
#include "Texture.h"
#include "Size.h"
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_ROCKET

#define GL_CLAMP_TO_EDGE 0x812F

void EdenRocketRenderInterface::RenderGeometry(
      Rml::Core::Vertex* vertices,
      int numVertices, int* indices, int numIndices,
      const Rml::Core::TextureHandle handle,
      const Rml::Core::Vector2f& translation)
{
   auto texture = reinterpret_cast<Texture*>(handle);

   glPushMatrix();
   glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
   glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

   glTranslatef(translation.x, translation.y, 0);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glVertexPointer(2, GL_FLOAT, sizeof(Rml::Core::Vertex), &vertices[0].position);
   glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Rml::Core::Vertex), &vertices[0].colour);

   if (!texture)
   {
      glDisable(GL_TEXTURE_2D);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }
   else
   {
      glEnable(GL_TEXTURE_2D);
      texture->bind();
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, sizeof(Rml::Core::Vertex), &vertices[0].tex_coord);
   }

   glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

   glPopAttrib();
   glPopClientAttrib();
   glPopMatrix();
}

void EdenRocketRenderInterface::EnableScissorRegion(bool enable)
{
   glDisable(GL_STENCIL_TEST);
   if (enable)
   {
      glEnable(GL_SCISSOR_TEST);
   }
   else
   {
      glDisable(GL_SCISSOR_TEST);
   }
}

void EdenRocketRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
   glScissor(x, GraphicsUtil::getInstance()->getHeight() - (y + height), width,
         height);
}

bool EdenRocketRenderInterface::LoadTexture(
      Rml::Core::TextureHandle& textureHandle,
      Rml::Core::Vector2i& textureDimensions,
      const Rml::Core::String& source)
{
   Texture* texture = new Texture(std::string(source.c_str()));
   if(!texture->isValid())
   {
      delete texture;
      return false;
   }

   textureHandle = reinterpret_cast<Rml::Core::TextureHandle>(texture);
   return true;
}

bool EdenRocketRenderInterface::GenerateTexture(
      Rml::Core::TextureHandle& textureHandle,
      const Rml::Core::byte* source,
      const Rml::Core::Vector2i& sourceDimensions)
{
   Texture* texture = new Texture(reinterpret_cast<const void*>(source),
         GL_RGBA, geometry::Size(sourceDimensions.x, sourceDimensions.y), 4);

   texture->bind();
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   textureHandle = reinterpret_cast<Rml::Core::TextureHandle>(texture);
   return true;
}

void EdenRocketRenderInterface::ReleaseTexture(Rml::Core::TextureHandle textureHandle)
{
   delete reinterpret_cast<Texture*>(textureHandle);
}

