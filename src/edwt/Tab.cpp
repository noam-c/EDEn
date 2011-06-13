#include "Tab.h"

namespace edwt
{
   void Tab::setBackgroundColor(gcn::Color color)
   {
      gcn::Tab::setBackgroundColor(color);
      mLabel->setBackgroundColor(color);
   }

   void Tab::setBaseColor(gcn::Color color)
   {
      gcn::Tab::setBaseColor(color);
      mLabel->setBaseColor(color);
   }

   void Tab::setForegroundColor(gcn::Color color)
   {
      gcn::Tab::setForegroundColor(color);
      mLabel->setForegroundColor(color);
   }
};