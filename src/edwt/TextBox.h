/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "guichan.hpp"
#include "TextAlignment.h"

namespace edwt
{
   /**
    * Overrides the original Guichan Text Box to add text alignment (LEFT, CENTER, RIGHT).
    *
    * @author Noam Chitayat
    */
   class TextBox : public gcn::TextBox
   {
      private:
         /** The text alignment of this TextBox */
         TextAlignment align;

         /** The text color of the TextBox */
         gcn::Color textColor;

         /** Determine the point in the x-axis where the text begins */
         int determineX(const std::string& text);

      protected:
         /**
          * Size adjustment is overridden to retain current width.
          * (unless the existing text does not fit in the current width)
          * This is necessary so that text updates don't change the width
          * (and thus affect the alignment) of the text in the box.
          */
         virtual void adjustSize();

      public:
         /**
          * Constructor.
          *
          * Text is left-aligned by default.
          */
         TextBox() : align(LEFT) {}

         /**
          * Scroll to the bottom row of the text box.
          */
         void scrollToBottom();

         /**
          * @param alignment The new text alignment of the text box.
          */
         void setAlignment(TextAlignment alignment);

         /**
          * @return the current text alignment of this text box.
          */
         TextAlignment getAlignment();

         /**
          * Border drawing is overridden to prevent drawing
          * when the text box is transparent.
          * (otherwise you still see the border all the time)
          *
          * @param graphics The graphics driver to draw with.
          */
         void drawBorder(gcn::Graphics* graphics);

         /**
          * Override the drawing code to take text alignment into account.
          * (most of the code is the same as the overridden implementation, sadly)
          *
          * @param graphics The graphics driver to draw with.
          */
         void draw(gcn::Graphics* graphics);
   };
};

#endif
