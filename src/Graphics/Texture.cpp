/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Texture.h"
#include "GraphicsUtil.h"
#include <SDL.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "Size.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_GRAPHICS

Texture::Texture() = default;

Texture::Texture(const std::string& imagePath)
{
   // Create the texture
   DEBUG("Generating texture...");
   glGenTextures(1, &m_textureHandle);

   // Create storage space for the texture and load the image
   DEBUG("Loading image %s...", imagePath.c_str());
   SDL_Surface *image = IMG_Load(imagePath.c_str());

   if (!image)
   {
      // Problem loading the image; throw an exception
      DEBUG("Unable to load image: %s", IMG_GetError());
      return;
   }

   DEBUG("Image load successful!");

   m_size.width = image->w;
   m_size.height = image->h;

   DEBUG("Binding GL texture");

   // Bind this texture as the current texture OpenGL should work with
   bind();

   // Add Linear filtering for the texture
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   GLenum textureFormat = GL_RGB;
   if (image->format->BytesPerPixel == 4) // contains an alpha channel
   {
      textureFormat = image->format->Rmask == 0x000000ff ? GL_RGBA : GL_BGRA;
   }
   else if (image->format->BytesPerPixel == 3) // no alpha channel
   {
      textureFormat = image->format->Rmask == 0x000000ff ? GL_RGB : GL_BGR;
   }

   // Transfer the image data into the texture
   DEBUG("Transferring image data to GL texture");
   glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel, m_size.width, m_size.height, 0, textureFormat, GL_UNSIGNED_BYTE, image->pixels);
   m_valid = true;

   DEBUG("Freeing image surface");
   SDL_FreeSurface(image);

   DEBUG("Texture creation complete.");
}

Texture::Texture(const void* imageData, GLenum imageFormat, const geometry::Size& imageSize, int bytesPerPixel) :
   m_size(imageSize)
{
   // Create the texture
   DEBUG("Generating texture...");
   glGenTextures(1, &m_textureHandle);

   // Bind the texture to alter its parameters
   bind();

   // Add Linear filtering for the texture
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Transfer the image data into the texture
   DEBUG("Transferring image data to GL texture");
   glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel, m_size.width, m_size.height, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);
   m_valid = true;

   DEBUG("Texture creation complete.");
}

Texture::Texture(Texture&& rhs)
{
   if(rhs.m_valid)
   {
      std::swap(m_valid, rhs.m_valid);
      std::swap(m_size, rhs.m_size);
      std::swap(m_textureHandle, rhs.m_textureHandle);
   }
}

Texture& Texture::operator=(Texture&& rhs)
{
   if(rhs.m_valid)
   {
      m_valid = rhs.m_valid;
      m_size = rhs.m_size;
      m_textureHandle = rhs.m_textureHandle;

      rhs.m_valid = false;
      rhs.m_textureHandle = 0;
   }

   return *this;
}

void Texture::bind()
{
   // Any texture ops on GL_TEXTURE_2D will become associated with this texture
   glBindTexture(GL_TEXTURE_2D, m_textureHandle);
}

bool Texture::isValid() const
{
   return m_valid;
}

const geometry::Size& Texture::getSize() const
{
   return m_size;
}

Texture::~Texture()
{
   if(m_textureHandle != 0)
   {
      glDeleteTextures(1, &m_textureHandle);
   }
}
