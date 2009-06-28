#include "Sprite.h"
#include "Spritesheet.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

Sprite::Sprite(Spritesheet* sheet) : sheet(sheet)
{
}

void Sprite::setFrame(std::string frameName)
{  Frame* newFrame = sheet->getFrame(frameName);
   if(newFrame)
   {  frame = newFrame;
      return;
   }

   T_T("Failed to find sprite frame.");
}

void Sprite::step(long timePassed)
{  
}

void Sprite::draw(int x, int y)
{  sheet->draw(x, y, frame);
}
