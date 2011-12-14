/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef TAB_CHANGE_LISTENER_H
#define TAB_CHANGE_LISTENER_H

#include <string>

namespace edwt
{
   /**
    * An interface for classes that wish to catch tab change events from a TabbedArea.
    *
    * @author Noam Chitayat
    */
   class TabChangeListener
   {
      public:
         
         /**
          * Called when a new tab in a tabbed area is selected.
          *
          * @param tabName the name of the tab that was selected.
          */
         virtual void tabChanged(const std::string& tabName) = 0;
   };
};

#endif
