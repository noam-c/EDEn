/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef SCREEN_TEXTURE_H
#define SCREEN_TEXTURE_H

typedef unsigned int GLuint;

#include "Texture.h"

class ScreenTexture : public Texture
{
   protected:
      GLuint frameBuffer;

   public:
      ScreenTexture();
      void startCapture();
      void endCapture();
      ~ScreenTexture();
};

#endif
