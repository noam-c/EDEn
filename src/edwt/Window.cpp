/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Window.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   Window::Window() : gcn::Window()
   {
   }

   Window::Window(const std::string& caption) : gcn::Window(caption)
   {
   }
   
   void Window::mousePressed(gcn::MouseEvent& mouseEvent)
   {
      gcn::Window::mousePressed(mouseEvent);
      int mouseY = mouseEvent.getY();
      mMoved = mouseY >= 0 && mouseY <= (int)mTitleBarHeight;
   }
};
