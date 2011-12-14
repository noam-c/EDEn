/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Container.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_EDWT;

namespace edwt
{
   Container::Container() : gcn::Container()
   {
   }
   
   bool Container::containsWidget(const gcn::Widget* widget)
   {
      WidgetListIterator iter;
      for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++)
      {
         if (*iter == widget)
         {
            return true;
         }
         
         Container* container = dynamic_cast<Container*>(*iter);
         if (container != NULL && container->containsWidget(widget))
         {
            return true;
         }
      }
      
      return false;
   }
};
