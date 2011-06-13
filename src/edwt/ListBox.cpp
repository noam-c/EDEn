#include "ListBox.h"
#include "StringListModel.h"
#include <SDL.h>
#include "Sound.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   ListBox::ListBox()
   {
      init();
   }

   ListBox::ListBox(edwt::StringListModel* listModel) : gcn::ListBox(listModel)
   {
      init();
   }

   void ListBox::init()
   {
      reselectSound = NULL;
      chooseSound = NULL;
      mOpaque = true;
      mPadding = 0;
      addMouseListener(this);
      addActionListener(this);
      setWrappingEnabled(true);
      gcn::ListBox::setSelected(0);
   }

   void ListBox::draw(gcn::Graphics* graphics)
   {
      if(mOpaque)
      {
         // Draw a border.
         graphics->setColor(0xFFFFFF);
         graphics->drawLine(0,
                           0,
                           0,
                           getHeight() - 2);
         graphics->setColor(0xCFCFCF);
         graphics->drawLine(getWidth() - 1,
                           1,
                           getWidth() - 1,
                           getHeight() - 1);
         graphics->drawLine(1,
                           getHeight() - 1,
                           getWidth() - 1,
                           getHeight() - 1);
         
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
      int y = getRowPadding() >> 1;

      fontHeight = getRowHeight();
      gcn::Color highlight(128,128,128);
      gcn::Color base(255,255,255);
      graphics->setColor(base);
      
      for (i = 0; i < mListModel->getNumberOfElements(); ++i)
      {
         const std::string& currString = mListModel->getElementAt(i);
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

	unsigned int ListBox::getRowHeight() const
	{
      return gcn::ListBox::getRowHeight() + getRowPadding();
	}

	unsigned int ListBox::getRowPadding() const
	{
		return mPadding;
	}

	void ListBox::setRowPadding(unsigned int padding)
	{
		mPadding = padding;
	}

   bool ListBox::isOpaque()
   {
      return mOpaque;
   }

   void ListBox::setOpaque(bool opaque)
   {
      mOpaque = opaque;
   }

   void ListBox::setReselectSound(Sound* newReselect)
   {
      reselectSound = newReselect;
   }

   void ListBox::adjustWidth()
   {
      //Find the maximum string width needed in the list box, and set the width to that       
      int maxWidth = 0;
      int itemWidth = 0;

      gcn::Font* font = getFont();

      //At each iteration, get the next item on the list and find its width relative to the font in use
      for (int i = 0; i < mListModel->getNumberOfElements(); ++i)
      {
         itemWidth = font->getWidth(mListModel->getElementAt(i));
         if(maxWidth < itemWidth)  {  maxWidth = itemWidth;  }
      }

      setWidth(maxWidth);
   }
   
   void ListBox::mouseMoved(gcn::MouseEvent& mouseEvent)
   {
      setSelected(mouseEvent.getY() / getFont()->getHeight());
   }

   void ListBox::setSelected(int selected)
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

   void ListBox::action(const gcn::ActionEvent&/*actionEvent*/)
   {
      SDL_Event event;
      event.type = SDL_USEREVENT;
      event.user.code = ((StringListModel*)mListModel)->getActionAt(getSelected());
      event.user.data1 = this;
      event.user.data2 = 0;
      SDL_PushEvent(&event);
   }

};