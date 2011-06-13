#ifndef TABBED_AREA_H
#define TABBED_AREA_H

#include "guichan.hpp"

namespace edwt
{
   class TabbedArea : public gcn::TabbedArea
   {
      public:
         virtual void setBackgroundColor(gcn::Color color);
         virtual void setBaseColor(gcn::Color color);
         virtual void setForegroundColor(gcn::Color color);
         virtual void setOpaque(bool opaque);
         virtual void addTab(const std::string& caption, gcn::Widget* widget);
         virtual void addTab(gcn::Tab* tab, gcn::Widget* widget);
         int getTabHeight() const;
   };
};
#endif