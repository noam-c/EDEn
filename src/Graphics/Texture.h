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

class Texture
{
   /** The texture handle */
   GLuint textureHandle;

   /** Texture size (in pixels) */
   shapes::Size size;

   public:
      Texture(const std::string& imagePath);
      void bind();
      const shapes::Size& getSize();
      ~Texture();
};

#endif
