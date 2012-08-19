/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ListBox.h"
#include "StringListModel.h"
#include "Sound.h"
#include <sstream>

#include "DebugUtils.h"
const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   ListBox::ListBox()
   {
      init();
   }

   ListBox::ListBox(gcn::ListModel* listModel) : gcn::ListBox(listModel)
   {
      init();
   }

   void ListBox::init()
   {
      mOpaque = true;
      mRowPadding = 0;
      mColumnPadding = 0;
      setNumColumns(1);
      setColumnAlignment(0, LEFT);

      setWrappingEnabled(true);
      setHighlightColor(gcn::Color(128,128,128));
   }
   
   unsigned int ListBox::getNumColumns()
   {
      return mColumns;
   }
   
   void ListBox::setNumColumns(unsigned int numColumns)
   {
      if(numColumns == 0)
      {
         throw GCN_EXCEPTION("ListBox needs to have at least one column.");
      }

      mColumns = numColumns;
      mMinColumnWidths.resize(numColumns);
      mColumnAlignments.resize(numColumns);
   }
   
   unsigned int ListBox::getMinColumnWidth(unsigned int column)
   {
      if (column >= mColumns)
      {
         throw GCN_EXCEPTION("ListBox column index out of bounds.");
      }
      else
      {
         return mMinColumnWidths[column];
      }
   }
   
   void ListBox::setMinColumnWidth(unsigned int column, unsigned int minWidth)
   {
      if (column >= mColumns)
      {
         throw GCN_EXCEPTION("ListBox column index out of bounds.");
      }
      else
      {
         mMinColumnWidths[column] = minWidth;
      }
   }
   
   void ListBox::setMinWidth(unsigned int minWidth)
   {
      setMinColumnWidth(0, minWidth);
   }
   
   TextAlignment ListBox::getColumnAlignment(unsigned int column)
   {
      if (column >= mColumns)
      {
         throw GCN_EXCEPTION("ListBox column index out of bounds.");
      }
      else
      {
         return mColumnAlignments[column];
      }
   }
   
   void ListBox::setColumnAlignment(unsigned int column, TextAlignment alignment)
   {
      if (column >= mColumns)
      {
         throw GCN_EXCEPTION("ListBox column index out of bounds.");
      }
      else
      {
         mColumnAlignments[column] = alignment;
      }
   }
   
   void ListBox::setAlignment(TextAlignment alignment)
   {
      setColumnAlignment(0, alignment);
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
      gcn::Color highlight = getHighlightColor();
      gcn::Color base = getBaseColor();
      graphics->setColor(base);

      for (i = 0; i < mListModel->getNumberOfElements(); ++i)
      {
         int columnBeginning = 0;
         std::string elementText = mListModel->getElementAt(i);
         std::stringstream columnText(elementText);
         
         for(unsigned int j = 0; j < mColumns; ++j)
         {
            std::string currString;
            if(!std::getline(columnText, currString, '\t')) break;

            int columnWidth = mColumnWidths[j] > mMinColumnWidths[j] ? mColumnWidths[j] : mMinColumnWidths[j];
            int xloc;
            
            switch(mColumnAlignments[j])
            {
               case RIGHT:
               {
                  xloc = columnBeginning + 1 + columnWidth - getFont()->getWidth(currString);
                  break;
               }
               case CENTER:
               {
                  xloc = columnBeginning + 1 + (columnWidth - getFont()->getWidth(currString))/2;
                  break;
               }
               case LEFT:
               default:
               {
                  xloc = columnBeginning + 1;
                  break;
               }
            }
            
            if (i == mSelected)
            {
               graphics->setColor(highlight);
               graphics->drawText(currString, xloc, y);
               graphics->setColor(base);
            }
            else
            {
               graphics->drawText(currString, xloc, y);
            }

            columnBeginning += columnWidth + getColumnPadding();
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
      return mRowPadding;
   }
   
   void ListBox::setRowPadding(unsigned int padding)
   {
      mRowPadding = padding;
   }
   
   unsigned int ListBox::getColumnPadding() const
   {
      return mColumnPadding;
   }
   
   void ListBox::setColumnPadding(unsigned int padding)
   {
      mColumnPadding = padding;
   }

   bool ListBox::isOpaque()
   {
      return mOpaque;
   }

   void ListBox::setOpaque(bool opaque)
   {
      mOpaque = opaque;
   }

   void ListBox::adjustWidth()
   {
      mColumnWidths.clear();
      mColumnWidths.resize(mColumns);

      // Find the maximum string width needed in each column, and set the widths to those maxima

      gcn::Font* font = getFont();

      // At each iteration, get the next item on the list and find its width relative to the font in use
      for (int i = 0; i < mListModel->getNumberOfElements(); ++i)
      {
         std::stringstream itemStream(mListModel->getElementAt(i));
         std::vector<unsigned int>::iterator iter = mColumnWidths.begin();
         std::vector<unsigned int>::iterator minWidthIter = mMinColumnWidths.begin();

         // Split the string into columns based on the presence of tab characters.
         std::string column;
         while(iter != mColumnWidths.end() && std::getline(itemStream, column, '\t'))
         {
            // Get the minimum width of the column.
            int minWidth = *minWidthIter;
            
            // Get the width of the column relative to the font.
            int itemWidth = font->getWidth(column);

            // If this item's width in the current column is larger, then set a new maximum.
            int currWidth = *iter;

            // If the current width is smaller than the item width, set the current maximum to the item width.
            currWidth = itemWidth < currWidth ? currWidth : itemWidth;
            
            // If the current width is smaller than the minimum width, set the current maximum to the minimum width.
            currWidth = minWidth < currWidth ? currWidth : minWidth;
            
            // Set the column's width to the current maximum. 
            *iter = currWidth;
            ++iter;
            ++minWidthIter;
         }
      }

      // Accumulate all the maximum column widths
      int maxWidth = 0;
      for(std::vector<unsigned int>::iterator iter = mColumnWidths.begin(); iter != mColumnWidths.end(); ++iter)
      {
         maxWidth += *iter;
      }
      
      // Add the width of all the padding
      maxWidth += getColumnPadding() * (mColumns - 1);

      // Set the new total list box width
      setWidth(maxWidth);
   }
   
   void ListBox::mouseMoved(gcn::MouseEvent& mouseEvent)
   {
      setSelected(mouseEvent.getY() / getRowHeight());
   }
   
   void ListBox::setSelected(int selected)
   {
      if(selected != mSelected)
      {
         gcn::ListBox::setSelected(selected);
      }
   }
   
   void ListBox::setHighlightColor(const gcn::Color& color)
   {
      highlightColor = color;
   }
   
   const gcn::Color& ListBox::getHighlightColor() const
   {
      return highlightColor;
   }

};
