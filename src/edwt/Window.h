#ifndef WINDOW_H
#define WINDOW_H

#include "guichan.hpp"

namespace edwt
{
   /**
    * Overrides the original Guichan window.
    *
    * @author Noam Chitayat
    */
   class Window : public gcn::Window
   {
      public:
         /**
          * Constructor.
          */
         Window();
   
         /**
          * Constructor.
          *
          * @param caption The window title bar caption.
          */
         Window(const std::string& caption);
      
         /**
          * Responds to mouse-click events.
          *
          * @param mouseEvent The mouse event to respond to.
          */ 
         void mousePressed(gcn::MouseEvent& mouseEvent);
   };
};

#endif
