#include "Sprite.h"
#include "Spritesheet.h"
#include "Animation.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

Sprite::Sprite(Spritesheet* sheet) : sheet(sheet), frameIndex(0), animated(false)
{
}

void Sprite::setSheet(Spritesheet* newSheet)
{
   sheet = newSheet;
}

void Sprite::setFrame(std::string frameName)
{
   int newFrameIndex = sheet->getFrameIndex(frameName);
   if(newFrameIndex < 0)
   {
      T_T("Failed to find sprite frame.");
   }

   frameIndex = newFrameIndex;
   animated = false;
}

void Sprite::setAnimation(std::string animationName)
{
   Animation* newAnimation = sheet->getAnimation(animationName);
   if(newAnimation == NULL)
   {
      T_T("Failed to find animation.");
   }

   animation = newAnimation;
   animated = true;
}

void Sprite::step(long timePassed)
{
   if(animated)
   {
      animation->update(timePassed);
   }
}

void Sprite::draw(int x, int y)
{
   int indexToDraw = animated ? animation->getIndex() : frameIndex;
   sheet->draw(x, y, indexToDraw);
}
