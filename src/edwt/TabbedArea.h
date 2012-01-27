/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TABBED_AREA_H
#define TABBED_AREA_H

#include "guichan.hpp"

namespace edwt
{
   class TabChangeListener;

   /**
    * Overrides the basic Guichan tabbed area to provide more control over look-and-feel, as well
    * as to enable owner classes to catch tab-change events as needed.
    *
    * @author Noam Chitayat
    */
   class TabbedArea : public gcn::TabbedArea
   {
      /**
       * The tab change listener for this tabbed area.
       * Currently, the TabbedArea only supports one listener at a time, but
       * this can be changed later if required.
       */
      TabChangeListener* tabChangeListener;

      public:
         /**
          * Constructor.
          */
         TabbedArea();
      
         /**
          * Updates the children of the tabbed area.
          */
         virtual void logic();
      
         /**
          * Set the background color of the tabbed area.
          *
          * @param color The new color to use.
          */
         virtual void setBackgroundColor(gcn::Color color);
            
         /**
          * Set the base color of the tabbed area.
          *
          * @param color The new color to use.
          */
         virtual void setBaseColor(gcn::Color color);

         /**
          * Set the foreground color of the tabbed area.
          *
          * @param color The new color to use.
          */
         virtual void setForegroundColor(gcn::Color color);

         /**
          * Sets whether or not the tab container should be opaque.
          * If the tabbed area is opaque, it will draw a background.
          * If not, the visual elements under the tabbed area will show through.
          *
          * @param opaque True iff the tabbed area should be opaque.
          */
         virtual void setOpaque(bool opaque);
      
         /**
          * Set the tab that should be selected by default.
          *
          * @param tab The tab that should be selected. The tab used must be one that is in the Tabbed Area.
          */
         virtual void setSelectedTab(gcn::Tab* tab);
      
         /**
          * Set the listener to notify when the user selects a new tab.
          * The tabbed area currently supports one listener at a time, so this call
          * will disconnect the previous listener (if there was one).
          *
          * @param listener The new listener to notify.
          */
         virtual void setTabChangeListener(TabChangeListener* listener);
      
         /**
          * Add a new tab using text and a widget that represents its pane.
          *
          * @param caption The caption text for the new tab.
          * @param widget The GUI elements to be shown when the new tab is selected.
          */
         virtual void addTab(const std::string& caption, gcn::Widget* widget);
      
         /**
          * Add a new tab and a widget that represents its pane. Use this call when making
          * a customized tab instead of the tabbed area's default tab.
          *
          * @param tab The tab to add.
          * @param widget The GUI elements to be shown when the new tab is selected.
          */
         virtual void addTab(gcn::Tab* tab, gcn::Widget* widget);

         /**
          * @return The pixel height of the tabs.
          */
         int getTabHeight() const;
   };
};

#endif
