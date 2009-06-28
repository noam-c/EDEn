#ifndef __SPRITESHEET_H_
#define __SPRITESHEET_H_

#include "Resource.h"
#include <map>
#include <string>

class Frame;

/**
 * \todo Document.
 */
class Spritesheet : public Resource
{  static const std::string IMG_EXTENSION;
   static const std::string DATA_EXTENSION;

   std::map<std::string, Frame*> frames;

   public:
      Spritesheet(ResourceKey name, const char* path);
      void draw(int x, int y, Frame* frame);
      Frame* getFrame(std::string frameName);
      size_t getSize();
};

#endif
