#include "TitleScreenListBox.h"
#include "StringListModel.h"
#include <SDL.h>
#include "Sound.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   TitleScreenListBox::TitleScreenListBox()
   {
      init();
   }

   TitleScreenListBox::TitleScreenListBox(edwt::StringListModel* listModel) : gcn::ListBox(listModel)
   {
      init();
   }

   void TitleScreenListBox::init()
   {
      mOpaque = true;
      addMouseListener(this);
      addActionListener(this);
      setWrappingEnabled(true);
      gcn::ListBox::setSelected(0);
   }

   void TitleScreenListBox::draw(gcn::Graphics* graphics)
   {
      if(mOpaque)
      {
         graphics->setColor(getBackgroundColor());
         graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
      }

      if (mListModel == NULL)
      {
         return;
      }

      graphics->setColor(getForegroundColor());
      graphics->setFont(getFont());

      int i, fontHeight;
      int y = 0;

      fontHeight = getFont()->getHeight();
      gcn::Color highlight(128,128,128);
      gcn::Color base(255,255,255);
      graphics->setColor(base);
      
      for (i = 0; i < mListModel->getNumberOfElements(); ++i)
      {
         std::string currString = mListModel->getElementAt(i);
         int xloc = 1 + (getWidth() - getFont()->getWidth(currString))/2;

         if (i == mSelected)
         {  graphics->setColor(highlight);
            graphics->drawText(currString, xloc, y);
            graphics->setColor(base);
         }
         else
         {  graphics->drawText(currString, xloc, y);
         }

         y += fontHeight;
      }
   }

   bool TitleScreenListBox::isOpaque()
   {
      return mOpaque;
   }

   void TitleScreenListBox::setOpaque(bool opaque)
   {
      mOpaque = opaque;
   }

   void TitleScreenListBox::setReselectSound(Sound* newReselect)
   {
      reselectSound = newReselect;
   }

   void TitleScreenListBox::adjustWidth()
   {
      //Find the maximum string width needed in the list box, and set the width to that       
      int maxWidth = 0;
      int itemWidth = 0;

      gcn::Font* font = getFont();

      //At each iteration, get the next item on the list and find its width relative to the font in use
      for (int i = 0; i < mListModel->getNumberOfElements(); itemWidth = font->getWidth(mListModel->getElementAt(i++)))
      {
         if(maxWidth < itemWidth)  {  maxWidth = itemWidth;  }
      }

      setWidth(maxWidth);
   }
   
   void TitleScreenListBox::mouseMoved(gcn::MouseEvent& mouseEvent)
   {
      setSelected(mouseEvent.getY() / getFont()->getHeight());
   }

   void TitleScreenListBox::setSelected(int selected)
   {
      if(getSelected() != selected)
      {
         if(reselectSound != NULL)
         {
            reselectSound->play();
         }
      }

      gcn::ListBox::setSelected(selected);
   }

   void TitleScreenListBox::action(const gcn::ActionEvent&/*actionEvent*/)
   {
      SDL_Event event;
      event.type = SDL_USEREVENT;
      event.user.code = ((StringListModel*)mListModel)->getActionAt(getSelected());
      event.user.data1 = this;
      event.user.data2 = 0;
      SDL_PushEvent(&event);
   }

};
