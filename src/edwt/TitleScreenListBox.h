#ifndef __LIST_BOX_H_
#define __LIST_BOX_H_

#include "guichan.hpp"

class Sound;

namespace edwt
{
   class StringListModel;

   /** 
    *  A widget that overrides the original Guichan List Box to add 
    *  transparency, mouse hover response (including sound), and SDL Events
    *
    *  @author Noam Chitayat
    */
   class TitleScreenListBox : public gcn::ListBox, public gcn::ActionListener
   {
      /** Tells the GUI whether or not to draw the list box transparently */
      bool mOpaque;

      /** A reselection sound when the mouse or keyboard changes the selected option */
      Sound* reselectSound;

      /** A sound that plays when the user chooses the option selected */
      Sound* chooseSound;

      public:
         /**
          * Constructor.
          */
         TitleScreenListBox();

         /**
          * Constructor.
          *
          * @param listModel the list model that holds the options for this list.
          */
         TitleScreenListBox(edwt::StringListModel *listModel);

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
          * @param newChoose The new choose sound.
          */
         void setChooseSound(Sound* newChoose);

         /**
          * @param newReselect The new reselection sound.
          */
         void setReselectSound(Sound* newReselect);

         /**
          * Adjust the width of the list box
          * (used after options are added to ensure that they fit)
          */
         void adjustWidth();

         /**
          * Change the selected option in the list box.
          *
          * @param selected The index of the option that should be selected.
          */
         void setSelected(int selected);

         /**
          * Send an SDL event to indicate that some action should be performed.
          * Occurs when an option is chosen by the user.
          *
          * @param actionEvent The selection event triggered by the user. (unused)
          */
         void action(const gcn::ActionEvent& actionEvent);
    };
};

#endif
