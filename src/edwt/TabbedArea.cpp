#include "TabbedArea.h"
#include "Tab.h"

namespace edwt
{
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
      gcn::TabbedArea::addTab(tab, widget);
   }

   int TabbedArea::getTabHeight() const
   {
      return mTabContainer->getHeight();
   }
};