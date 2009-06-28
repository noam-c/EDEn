#ifndef __SPRITE_H_
#define __SPRITE_H_

#include <string>

class Frame;
class Spritesheet;

class Sprite
{  Spritesheet* sheet;
   Frame* frame;

   public:
      Sprite(Spritesheet* sheet);
      void setFrame(std::string frameName);
      void step(long timePassed);
      void draw(int x, int y);
};

#endif
