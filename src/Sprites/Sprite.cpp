#include "Sprite.h"
#include "Spritesheet.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

Sprite::Sprite(Spritesheet* sheet) : sheet(sheet), frameIndex(0)
{
}

void Sprite::setFrame(std::string frameName)
{  int newFrameIndex = sheet->getFrameIndex(frameName);
   if(newFrameIndex < 0)
   {  T_T("Failed to find sprite frame.");
   }

   frameIndex = newFrameIndex;
}

void Sprite::step(long timePassed)
{  
}

void Sprite::draw(int x, int y)
{  sheet->draw(x, y, frameIndex);
}
