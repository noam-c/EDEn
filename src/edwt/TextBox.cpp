#include "TextBox.h"
#include "DebugUtils.h"
#include <SDL.h>
#include "SDL_mixer.h"
#include "StringListModel.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   void TextBox::drawBorder(gcn::Graphics* graphics)
   {
      if(mOpaque) gcn::TextBox::drawFrame(graphics);
   }

   void TextBox::draw(gcn::Graphics* graphics)
   {
      if (mOpaque)
      {
         graphics->setColor(getBackgroundColor());
         graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
      }
   
      if (isFocused() && isEditable())
      {
         drawCaret(graphics, getFont()->getWidth(mTextRows[mCaretRow].substr(0, mCaretColumn)), mCaretRow * getFont()->getHeight());
      }
    
      graphics->setColor(getForegroundColor());
      graphics->setFont(getFont());
   
      for (unsigned int i = 0; i < mTextRows.size(); i++)
      {
         graphics->drawText(mTextRows[i], determineX(mTextRows[i]), i * getFont()->getHeight());
      }
   }
   
   void TextBox::adjustSize()
   {
      int width = getWidth() - 1;
      for (unsigned int i = 0; i < mTextRows.size(); ++i)
      {
         int w = getFont()->getWidth(mTextRows[i]);
         if (width < w)
         {
            width = w;
         }
      }
   
      setWidth(width + 1);
      setHeight(getFont()->getHeight() * mTextRows.size());
   }
   
   int TextBox::determineX(const std::string& text)
   {
      switch(align)
      {
         case CENTER:
         {
            return (getWidth() - getFont()->getWidth(text)) / 2;
         }
         case RIGHT:
         {
            return (getWidth() - getFont()->getWidth(text)) - 1;
         }
         case LEFT:
         default:
         {
            return 1;
         }   
      }
   }
   
   void TextBox::scrollToBottom()
   {
      gcn::Rectangle scroll;
      scroll.x = 0;
      scroll.y = getFont()->getHeight() * mTextRows.size();
      scroll.width = getFont()->getWidth(" ");
      scroll.height = getFont()->getHeight() + 2;
      showPart(scroll);
   }

   void TextBox::setAlignment(TextAlignment alignment)
   {
      align = alignment;
   }
   
   TextAlignment TextBox::getAlignment()
   {
      return align;
   }
};
