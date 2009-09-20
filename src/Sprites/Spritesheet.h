#ifndef __SPRITESHEET_H_
#define __SPRITESHEET_H_

#include "Resource.h"
#include <map>
#include <string>

class Frame;

typedef unsigned int GLuint;

/**
 * \todo Document.
 */
class Spritesheet : public Resource
{
   static const std::string IMG_EXTENSION;
   static const std::string DATA_EXTENSION;

   /** The spritesheet texture */
   GLuint texture;

   /** Width (in pixels) */
   int width;

   /** Height (in pixels) */
   int height;

   int numFrames;

   std::map<std::string, int> frameIndices;

   Frame** frameList;

   void load(const char* path);

   public:
      Spritesheet(ResourceKey name);
      void draw(int x, int y, int frameIndex);
      int getFrameIndex(std::string frameName);
      size_t getSize();
};

#endif
