#ifndef TAB_H
#define TAB_H

#include "guichan.hpp"

namespace edwt
{
   class Tab : public gcn::Tab
   {
      public:
         virtual void setBackgroundColor(gcn::Color color);
         virtual void setBaseColor(gcn::Color color);
         virtual void setForegroundColor(gcn::Color color);
   };
};
#endif