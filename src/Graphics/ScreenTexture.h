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
