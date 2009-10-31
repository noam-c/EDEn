#ifndef __SPRITE_H_
#define __SPRITE_H_

#include <string>

class Spritesheet;
class Animation;

class Sprite
{
   Spritesheet* sheet;
   int frameIndex;
   Animation* animation;
   bool animated;

   public:
      Sprite(Spritesheet* sheet);
      void setSheet(Spritesheet* newSheet);
      void setFrame(std::string frameName);
      void setAnimation(std::string animationName);
      void step(long timePassed);
      void draw(int x, int y);
};

#endif
