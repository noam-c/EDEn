#ifndef TAB_CHANGE_LISTENER_H
#define TAB_CHANGE_LISTENER_H

#include <string>

namespace edwt
{
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