#include "Sprite.h"
#include "Spritesheet.h"
#include "Animation.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

Sprite::Sprite(Spritesheet* sheet) : sheet(sheet), frameIndex(0), animation(NULL)
{
}

void Sprite::clearCurrentFrame()
{
   if(animation)
   {
      delete animation;
      animation = NULL;
   }

   // Default to frame 0 for now.
   frameIndex = 0;
}

void Sprite::setSheet(Spritesheet* newSheet)
{
   sheet = newSheet;

   // A new sheet invalidates the current frame information.
   clearCurrentFrame();
}

void Sprite::setFrame(std::string frameName)
{
   int newFrameIndex = sheet->getFrameIndex(frameName);
   if(newFrameIndex < 0)
   {
      T_T("Failed to find sprite frame.");
   }

   clearCurrentFrame();
   frameIndex = newFrameIndex;
}

void Sprite::setAnimation(std::string animationName)
{
   Animation* newAnimation = sheet->getAnimation(animationName);
   if(newAnimation == NULL)
   {
      T_T("Failed to find animation.");
   }

   clearCurrentFrame();
   animation = newAnimation;
}

void Sprite::step(long timePassed)
{
   if(animation != NULL)
   {
      animation->update(timePassed);
   }
}

void Sprite::draw(int x, int y)
{
   int indexToDraw = animation != NULL ? animation->getIndex() : frameIndex;
   sheet->draw(x, y, indexToDraw);
}

Sprite::~Sprite()
{
   clearCurrentFrame();
}