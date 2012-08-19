/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MODULE_SELECT_LISTENER_H
#define MODULE_SELECT_LISTENER_H

#include <string>

namespace edwt
{
   /**
    * An interface for classes that detect and act on savegame selection events in the in-game menu.
    *
    * @author Noam Chitayat
    */
   class ModuleSelectListener
   {
      public:
         /**
          * Fires when a module is selected from a list in the in-game menu.
          *
          * @param index The index of the GUI module that was selected.
          */
         virtual void moduleSelected(int index, const std::string& eventId) = 0;
   };
};
#endif
