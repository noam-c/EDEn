#ifndef LIST_BOX_H_
#define LIST_BOX_H_

#include "guichan.hpp"

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
      unsigned int mPadding;

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
         * @param padding The new padding to set (in pixels)
         */
        virtual void setRowPadding(unsigned int padding);

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
   };
};

#endif
