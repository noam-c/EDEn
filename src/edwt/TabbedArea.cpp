/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "TabbedArea.h"
#include "Tab.h"
#include "TabChangeListener.h"

namespace edwt
{
   TabbedArea::TabbedArea() : tabChangeListener(NULL) {}

   void TabbedArea::logic()
   {
      logicChildren();
   }

   void TabbedArea::setBackgroundColor(gcn::Color color)
   {
      gcn::TabbedArea::setBackgroundColor(color);
      mTabContainer->setBackgroundColor(color);
      mWidgetContainer->setBackgroundColor(color);
   }

   void TabbedArea::setBaseColor(gcn::Color color)
   {
      gcn::TabbedArea::setBaseColor(color);
      mTabContainer->setBaseColor(color);
      mWidgetContainer->setBaseColor(color);
   }

   void TabbedArea::setForegroundColor(gcn::Color color)
   {
      gcn::TabbedArea::setForegroundColor(color);
      mTabContainer->setForegroundColor(color);
      mWidgetContainer->setForegroundColor(color);
   }

   void TabbedArea::setOpaque(bool opaque)
   {
      gcn::TabbedArea::setOpaque(opaque);
      mTabContainer->setOpaque(opaque);
      mWidgetContainer->setOpaque(opaque);
   }

   void TabbedArea::setSelectedTab(gcn::Tab* tab)
   {
      gcn::Tab* oldTab = gcn::TabbedArea::getSelectedTab();

      gcn::TabbedArea::setSelectedTab(tab);
      if (tab != oldTab && tabChangeListener != NULL)
      {
         tabChangeListener->tabChanged(tab->getCaption());
      }
   }

   void TabbedArea::setTabChangeListener(TabChangeListener* listener)
   {
      tabChangeListener = listener;
   }

   void TabbedArea::addTab(const std::string& caption, gcn::Widget* widget)
   {
        Tab* tab = new Tab();
        tab->setCaption(caption);

        tab->setBaseColor(getBaseColor());
        tab->setForegroundColor(getForegroundColor());
        tab->setBackgroundColor(getBackgroundColor());

        mTabsToDelete.push_back(tab);

        addTab(tab, widget);
   }

   void TabbedArea::addTab(gcn::Tab* tab, gcn::Widget* widget)
   {
        tab->setTabbedArea(this);
        tab->addActionListener(this);

        mTabContainer->add(tab);
        mTabs.push_back(std::pair<gcn::Tab*, Widget*>(tab, widget));

        if (mSelectedTab == NULL)
        {
            gcn::TabbedArea::setSelectedTab(tab);
        }

        adjustTabPositions();
        adjustSize();
   }

   int TabbedArea::getTabHeight() const
   {
      return mTabContainer->getHeight();
   }
};
