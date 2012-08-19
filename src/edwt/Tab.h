/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TAB_H
#define TAB_H

#include "guichan.hpp"

namespace edwt
{
   /**
    * Overrides the Guichan Tab to provide more control over the look-and-feel of the Tab.
    *
    * @author Noam Chitayat
    */
   class Tab : public gcn::Tab
   {
      public:
         /**
          * Change the background color of the entire tab.
          *
          * @param color The new color to use.
          */
         virtual void setBackgroundColor(gcn::Color color);
      
         /**
          * Change the base color of the tab (which includes the text color for the tab label).
          *
          * @param color The new color to use.
          */
         virtual void setBaseColor(gcn::Color color);
      
         /**
          * Set the foreground color of the entire tab.
          *
          * @param color The new color to use.
          */
         virtual void setForegroundColor(gcn::Color color);
   };
};
#endif
