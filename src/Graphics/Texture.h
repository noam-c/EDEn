/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "Size.h"

typedef unsigned int GLuint;

/**
 * Manages the creation, binding and deletion of an OpenGL texture.
 *
 * @author Noam Chitayat
 */
class Texture
{
   protected:
      /** The texture handle */
      GLuint textureHandle;

      /** Texture size (in pixels) */
      shapes::Size size;

      /** Default constructor (used when subclass initializes the texture. */
      Texture();

   public:
      /**
       * Constructor.
       * Creates an OpenGL texture from a given image.
       *
       * @param imagePath the path to the image to load into the texture.
       */
      Texture(const std::string& imagePath);

      /**
       * Binds the texture to the drawing context for
       * modification or drawing.
       */
      void bind();

      /**
       * @return the dimensions of the texture in pixels.
       */
      const shapes::Size& getSize() const;

      /**
       * Destructor.
       */
      virtual ~Texture();
};

#endif
