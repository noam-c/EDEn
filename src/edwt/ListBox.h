/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef LIST_BOX_H
#define LIST_BOX_H

#include "guichan.hpp"
#include "TextAlignment.h"

class Sound;

namespace edwt
{
   class StringListModel;

   /** 
    *  A widget that overrides the original Guichan List Box to add 
    *  transparency, padding and mouse hover reselection.
    *
    *  @author Noam Chitayat
    */
   class ListBox : public gcn::ListBox
   {
      /** Tells the GUI whether or not to draw the list box transparently */
      bool mOpaque;
      
      /** The padding between items in the list */
      unsigned int mRowPadding;
      
      /** The padding between columns in the list */
      unsigned int mColumnPadding;
      
      /** The number of columns in the list */
      unsigned int mColumns;
      
      /** The widths of the list columns */
      std::vector<unsigned int> mColumnWidths;
      
      /** The widths of the list columns */
      std::vector<unsigned int> mMinColumnWidths;
      
      /** The widths of the list columns */
      std::vector<TextAlignment> mColumnAlignments;
      
      /** The color to use when drawing a selected (highlighted) element */
      gcn::Color highlightColor;

      public:
         /**
          * Constructor.
          */
         ListBox();

         /**
          * Constructor.
          *
          * @param listModel the list model that holds the options for this list.
          */
         ListBox(gcn::ListModel *listModel);

         /**
          * Initialization of the list's basic properties.
          */
         void init();
         
         /**
          * @return The number of columns in this list box.
          */
         unsigned int getNumColumns();
         
         /**
          * Sets the number of columns in this list box.
          *
          * @param numColumns The number of columns that this list box will display.
          * @throws gcn::Exception if numColumns is 0, since a list cannot have no columns.
          */
         void setNumColumns(unsigned int numColumns);

         /**
          * Gets the minimum width of a specified column.
          *
          * @param column The column to check.
          * @return The minimum width of the specified column.
          * @throws gcn::Exception if the specified column is higher than the number of columns set.
          */
         unsigned int getMinColumnWidth(unsigned int column);

         /**
          * Sets the minimum width of a specified column.
          *
          * @param column The column to change.
          * @param minWidth The minimum width that the specified column should take.
          * @throws gcn::Exception if the specified column is higher than the number of columns set.
          */
         void setMinColumnWidth(unsigned int column, unsigned int minWidth);

         /**
          * Sets the minimum width of the list.
          *
          * @param minWidth The minimum width that the list should take.
          */
         void setMinWidth(unsigned int minWidth);
      
         /**
          * Gets the alignment of a specified column.
          *
          * @param column The column to check.
          * @return The alignment of the specified column.
          * @throws gcn::Exception if the specified column is higher than the number of columns set.
          */
         TextAlignment getColumnAlignment(unsigned int column);
         
         /**
          * Sets the minimum width of a specified column.
          *
          * @param column The column to change.
          * @param alignment The text alignment that the specified column should take.
          * @throws gcn::Exception if the specified column is higher than the number of columns set.
          */
         void setColumnAlignment(unsigned int column, TextAlignment alignment);
         
         /**
          * Sets the alignment of the list.
          *
          * @param alignment The alignment that the list should take.
          */
         void setAlignment(TextAlignment alignment);
         
         /**
          * Draw the list box. 
          * Overridden to provide special drawing properties, including transparency.
          *
          * @param graphics The graphics driver to draw with.
          */
         void draw(gcn::Graphics* graphics);

        /**
         * Gets the height of a row. Overridden to pad the font height slightly.
         *
         * @return The height of a row.
         */
        virtual unsigned int getRowHeight() const;
      
         /**
          * Gets the padding between items.
          *
          * @return The padding between items.
          */
         virtual unsigned int getRowPadding() const;
         
         /**
          * Sets the padding between items.
          *
          * @param padding The new row padding to set (in pixels)
          */
         virtual void setRowPadding(unsigned int padding);
         
         /**
          * Gets the padding between columns.
          *
          * @return The padding between columns.
          */
         virtual unsigned int getColumnPadding() const;
         
         /**
          * Sets the padding between columns.
          *
          * @param padding The new column padding to set (in pixels)
          */
         virtual void setColumnPadding(unsigned int padding);

         /**
          * The mouse moved. Update the list box selection accordingly.
          *
          * @param mouseEvent The mouse event (contains movement and click info)
          */
         void mouseMoved(gcn::MouseEvent& mouseEvent);

         /**
          * @return true iff the list box is opaque.
          */
         bool isOpaque();

         /**
          * @param opaque true iff the list box should be drawn opaque.
          */
         void setOpaque(bool opaque);

         /**
          * Adjust the width of the list box
          * (used after options are added to ensure that they fit)
          */
         void adjustWidth();

         /**
          * Change the selected option in the list box.
          * Restricts selection events from firing if the
          * selected option hasn't changed.
          *
          * @param selected The index of the option that should be selected.
          */
         void setSelected(int selected);
      
         /**
          * Sets the highlight color.
          *
          * @param color The highlight color.
          */
         void setHighlightColor(const gcn::Color& color);
         
         /**
          * Gets the highlight color.
          */
         const gcn::Color& getHighlightColor() const;
   };
};

#endif
