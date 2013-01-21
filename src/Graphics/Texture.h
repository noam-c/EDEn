/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "Size.h"

struct SDL_Surface;
typedef unsigned int GLuint;
typedef unsigned int GLenum;

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

      /** True iff the texture was successfully generated */
      bool valid;

      /** Texture size (in pixels) */
      shapes::Size size;

      /** Default constructor (used when subclass initializes the texture). */
      Texture();

      /**
       * Generates an OpenGL texture from an SDL Surface.
       *
       * @param image The surface containing the image for the texture.
       */
      void initTextureFromImage(SDL_Surface* image);

   public:
      /**
       * Constructor.
       * Creates an OpenGL texture from a given image.
       *
       * @param imagePath the path to the image to load into the texture.
       */
      Texture(const std::string& imagePath);

      /**
       * Constructor.
       * Creates an OpenGL texture from image data in memory.
       *
       * @param imageData the pointer to the image data to load into the texture.
       * @param imageFormat the image data format (expressed as an OpenGL format GLenum)
       * @param imageSize the size of the image (in pixels)
       * @param bytesPerPixel the number of bytes representing each pixel in the image
       */
      Texture(const void* imageData, GLenum imageFormat, const shapes::Size& imageSize, int bytesPerPixel);

      /**
       * Binds the texture to the drawing context for
       * modification or drawing.
       */
      void bind();

      /**
       * @return true iff this texture object is valid.
       */
      bool isValid() const;

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
